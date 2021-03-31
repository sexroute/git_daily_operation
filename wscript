# -*- encoding: utf-8 -*-
# vim: syntax=python

import os
import sys
import re
import fnmatch
import glob
import locale
import shutil
import types
import urllib

from waflib      import Logs
from waflib      import Errors
from waflib      import Utils
from waflib      import Scripting

"""
- 兼容nant targets : version, build/rebuild, export
- 版本号定义定义可以为string 也可以为 file 还可以是 url
- 使用@depends确保依赖被调用，call直接调用其它targets
- 可使用--conf指定不同的conf文件，默认为wconf
- 可通过--product-version指定版本号
- 可通过--export_dir和--upload-url指定导出目录和上传地址
"""

### Dependes
def before(*targets):

    def _decorator(target):

        def _wrapper(context):
            if context.cmd in ('clean', 'install') :
                Logs.debug(u"避免clean使用build的依赖")
                target(context)
            else:
                for cmd in targets:
                    Logs.info("Depends for %s : '%s' Running ..." % (context.cmd, cmd))
                    call(cmd)

                Logs.info("Targets %s Running ..." % context.cmd)
                target(context)

        return _wrapper

    return _decorator

depends = before

def after(*targets):

    def _decorator(target):

        def _wrapper(context):
            if context.cmd in ('clean', 'install') :
                Logs.debug(u"避免clean/install使用build的依赖")
                target(context)
            else:
                Logs.info("Targets %s Running ..." % context.cmd)
                target(context)

            for cmd in targets:
                Logs.info("Registered target after %s : '%s' Running ..." % (context.cmd, cmd))
                call(cmd)


        return _wrapper

    return _decorator

def call(target):
    return Scripting.run_command(target)


def hook(target):
    "execute HOOK cmd in wconf when target called"

    def _call_hook_cmd(ctx):
        if not hasattr(WCONF, 'HOOK'):
            return

        if not WCONF.HOOK:
            return

        commands = WCONF.HOOK.get(ctx.cmd)

        if not commands:
            return

        for cmd in _make_list(commands):
            print "Run hook for %s ..." % ctx.cmd
            cmd(ctx)

    def _wrapper(context):
        if context.cmd in ('clean', 'install') :
            Logs.debug(u"避免clean/install使用build的依赖")
            target(context)
        else:
            Logs.info("Targets %s Running ..." % context.cmd)
            target(context)

        _call_hook_cmd(context)

    return _wrapper

### 辅助函数

def _utf8_to_local(u):
    return u.decode('utf-8').encode(locale.getpreferredencoding())

def _local_to_utf8(u):
    return u.decode(locale.getpreferredencoding()).encode('utf-8')

def _unicode_to_local(u):
    return u.encode(locale.getpreferredencoding())

def _local_to_unicode(u):
    return u.decode(locale.getpreferredencoding())

def _rstrip_from(url):
    return url[url.rindex('/') + 1:]

def _color_as_important(m):
    return u"%s%s%s" % (Logs.colors.RED, m, Logs.colors.NORMAL)

def _color_as_info(m):
    return u"%s%s%s" % (Logs.colors.YELLOW, m, Logs.colors.NORMAL)

def _writeln(msg):
    sys.stderr.write(msg + os.linesep)


def _parse_version(spec):
    if os.path.isfile(spec):
        root, ext = os.path.splitext(spec)
        if ext in ('.py', '.pyc'):
            import imp
            conf = imp.load_source('conf', spec)
            return _parse_version(conf.VERSION)
        else:
            with open(spec, "r") as f:
                version = f.readline()
    elif spec.startswith('http'):
        opener = urllib.FancyURLopener({})
        f = opener.open(spec)
        version = f.readline()
    else:
        version = spec

    return version.strip()

def _parse_rc_version(spec):
    p_info    = re.compile('^\s*?VS_VERSION_INFO\s+?VERSIONINFO')
    p_version = re.compile('^\s*?PRODUCTVERSION\s+?(?P<version>.*)$')
    p_digit   = re.compile('(?P<digit>\d+)')

    with open(spec) as rc:
      for line in rc:
        if not p_info.search(line):
          continue

        for line in rc:
          match = p_version.search(line)
          if match:
            version = match.group('version')
            return '.'.join([m.group('digit') for m in p_digit.finditer(version)])


def _copy_file(src, dst, basedir):
    assert os.path.isfile(src)

    if basedir:
        if not src.startswith(basedir):
            raise Errors.WafError("%r不在目录%r下")

        relative = src[len(basedir) + 1 : ]

        dst = os.path.join(dst, os.path.dirname(relative))

    if not os.path.isdir(dst):
        os.makedirs(dst)

    try:
        shutil.copy2(src, dst)
    except IOError:
        try:
            os.stat(src)
        except (OSError, IOError):
            Logs.error('File %r does not exist' % src)

        raise Errors.WafError('Could not copy %r' % src)

    #Logs.debug("  %s -> %s" % (src, dst))

    try:
        print "  %s -> %s" % (src, dst)
    except:
        print '  %r -> %r' % (src, dst)


def _make_list(item):
    if type(item) == types.ListType or type(item) == types.TupleType:
        return item
    else:
        return [item]

def _exclude(item):

    patterns = _make_list(WCONF.EXCLUDE)

    for p in patterns:
        if "\\" in p or '/' in p:
            if hasattr(p, 'match'):
                if p.match(item):
                    return True
            else:
                if fnmatch.fnmatch(item, p):
                    return True
        else:
            splits   = Utils.split_path(item)

            if hasattr(p, 'match'):
                if p.match(splits[-1]):
                    return True
            else:
                if fnmatch.fnmatch(splits[-1], p):
                    return True

    return False

def _run_msi(function):
    u"""
    调用msi静默安装
    function可能为subprocess.call 或 Context.exec_command等
    """

    def __run_cmd(filename):
        function('"%s" /i %s /qn' % (_get_msiexec(), filename))

    return __run_cmd


def _run_py(function):
    u"""
    调用python执行
    function可能为subprocess.call 或 Context.exec_command等
    """

    def __run_cmd(filename):
        function('"%s" %s' % (_get_python(), filename))

    return __run_cmd


def _install(url, fun):

    (filename, headers) = urllib.urlretrieve(url)

    try:
        fun(filename)
    except:
        Logs.error(u"安装%s失败" % url)

    finally:

        if os.path.exists(filename):
            os.remove(filename)

            return True


def _get_tortoiseproc():
    SubWCRev = Utils.get_registry_app_path(Utils.winreg.HKEY_LOCAL_MACHINE,
                                           ("SubWCRev",))

    if not SubWCRev:
        raise Errors.WafError(u"未安装TortoiseSVN")

    file_parts = Utils.split_path(SubWCRev)

    file_parts[-1] = "TortoiseProc.exe"

    TortoiseProc = os.sep.join(file_parts)

    if not os.path.exists(TortoiseProc):
        raise Errors.WafError(u"未找到TortoiseProc.exe")

    return TortoiseProc


def _get_python():
    python = Utils.get_registry_app_path(Utils.winreg.HKEY_LOCAL_MACHINE,
                                           ("python",))
    if not python:
        raise Errors.WafError(u"未安装python")

    if not os.path.exists(python):
        raise Errors.WafError(u"未找到python.exe")

    return python


def _get_msiexec():
    windows_dir = os.environ.get('SYSTEMROOT', r'C:\WINDOWS')

    msiexec = os.path.join(windows_dir, "system32", "msiexec.exe")

    if not os.path.exists(msiexec):
        raise Errors.WafError(u"未找到msiexec")

    return msiexec


def _get_explorer():
    windows_dir = os.environ.get('SYSTEMROOT', r'C:\WINDOWS')

    explorer = os.path.join(windows_dir, "explorer.exe")

    if not os.path.exists(explorer):
        raise Errors.WafError(u"未找到explorer")

    return explorer


def _create_build_command(solution, instruction="rebuild", configuration="release"):
    def _get_devenv():
        vs71commontools_dir = os.environ.get('VS71COMNTOOLS',
              r'C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\Tools')

        devenv = os.path.normpath(os.path.join(vs71commontools_dir, r'..\IDE\devenv.com'))

        if not os.path.exists(devenv):
            raise Errors.WafError(u"未找到devenv")

        return devenv

    def _get_vs2010_installdir():

        try:
            import _winreg
        except ImportError:
            return (False, "Can't import _winreg")

        subkey = (
            'SOFTWARE\\Microsoft\\VisualStudio\\SxS\\VS7')

        try:
            key = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE, subkey)
            try:
                return (True, _winreg.QueryValueEx(key, "10.0")[0])
            finally:
                key.Close()
        except WindowsError as e:
            return (False, str(e))


    def _get_vcvarsall():
        got, vs10installdir = _get_vs2010_installdir()
        if not got:
            raise Errors.WafError(u"未找到vcvarsall.bat: %s" % vs10installdir)

        vcvarsall = os.path.normpath(os.path.join(vs10installdir, r'VC\vcvarsall.bat'))

        if not os.path.exists(vcvarsall):
            raise Errors.WafError(u"未找到vcvarsall.bat")

        return vcvarsall

    def _get_msbuild():
        windows_dir = os.environ.get('SYSTEMROOT', r'C:\WINDOWS')

        dot_net_framework_dir = os.path.join(windows_dir, r'Microsoft.NET\Framework')

        msbuild_list = glob.glob(os.path.join(dot_net_framework_dir, "v4.0.*", "MSBuild.exe"))

        if len(msbuild_list) > 0:
            return msbuild_list[0]
        else:
            raise Errors.WafError(u"未找到MSBuild.exe")

    slnVersion = None
    with open(solution) as f:
        for line in f:
            m = re.match(r"Microsoft Visual Studio Solution File, Format Version ([\d.]+)", line.strip())
            if m:
                slnVersion = int(m.groups()[0].split('.')[0])
                break

    if slnVersion == 11: #  VS2010
        command = u'"%(builder)s" %(solution)s /t:%(instruction)s /p:Configuration=%(configuration)s' % {
                    "builder" : _get_msbuild(),
                    "solution" : solution,
                    "instruction" : instruction,
                    "configuration" : configuration }
    elif slnVersion == 8 : # VS2003
        command = u'"%(builder)s" %(solution)s /nologo /%(instruction)s %(configuration)s' % {
                    "builder" : _get_devenv(),
                    "solution" : solution,
                    "instruction" : instruction,
                    "configuration" : configuration }
    else:
        raise RuntimeError("Solution %s using version %d seems not supported!" % (solution, slnVersion))

    return command


### 检查必备工具

import pysvn

class SVNCommand(object):

    def __init__(self):
        self.client = pysvn.Client()

        self.revision_update_complete = None

        self.client.callback_notify = self.callback_notify
        self.client.callback_cancel = self.callback_cancel

        if hasattr( self.client, 'callback_conflict_resolver' ):
            self.client.callback_conflict_resolver = self.callback_conflict_resolver

        self.wc_notify_action_map = {
            pysvn.wc_notify_action.add:                    'A',
            pysvn.wc_notify_action.commit_added:           'A',
            pysvn.wc_notify_action.commit_deleted:         'D',
            pysvn.wc_notify_action.commit_modified:        'M',
            pysvn.wc_notify_action.commit_postfix_txdelta: None,
            pysvn.wc_notify_action.commit_replaced:        'R',
            pysvn.wc_notify_action.copy:                   'c',
            pysvn.wc_notify_action.delete:                 'D',
            pysvn.wc_notify_action.failed_revert:          'F',
            pysvn.wc_notify_action.resolved:               'R',
            pysvn.wc_notify_action.restore:                'R',
            pysvn.wc_notify_action.revert:                 'R',
            pysvn.wc_notify_action.skip:                   'skip',
            pysvn.wc_notify_action.status_completed:       None,
            pysvn.wc_notify_action.status_external:        'X',
            pysvn.wc_notify_action.update_add:             'A',
            pysvn.wc_notify_action.update_completed:       None,
            pysvn.wc_notify_action.update_delete:          'D',
            pysvn.wc_notify_action.update_external:        'X',
            pysvn.wc_notify_action.update_update:          'U',
            pysvn.wc_notify_action.annotate_revision:      'A',
            #pysvn.wc_notify_action.update_started:         'updating',
        }

    def callback_notify( self, arg_dict ):
        if arg_dict['action'] == pysvn.wc_notify_action.update_completed:
            self.revision_update_complete = arg_dict['revision']
        elif arg_dict['path'] != '' and self.wc_notify_action_map[ arg_dict['action'] ] is not None:
            msg = '%s %s' % (self.wc_notify_action_map[ arg_dict['action'] ], arg_dict['path'])

            print( msg )

    def callback_cancel( self ):
        return False

    def callback_conflict_resolver( self, arg_dict ):
        if arg_dict.has_key('my_file'):
            print '[R] %s' % arg_dict['my_file']

        return pysvn.wc_conflict_choice.theirs_full, None, False

    def get_instance(self):
        return self.client

    def get_update_revision(self):
        return self.revision_update_complete


# 全局变量
SVN = SVNCommand()

def _get_svn_wc_info(wc):
    svn = SVN.get_instance()

    if os.path.isdir(wc):
        wc_dir = wc
    else:
        wc_dir = os.path.dirname(wc)

    try:
        svn_info = svn.info(wc_dir)
    except pysvn.ClientError, e:
        raise Errors.WafError(u"读取工作拷贝%s失败 : %s" % (wc_dir, e))

    return svn_info

def _get_svn_changed_revision(wc):

    def _match(path_pattern):
        return lambda path_name: path_name.split("/")[-1] == path_pattern

    def _ext_is(path_ext):
        return lambda path_name: path_name.split(".")[-1] == path_ext

    ignore_pattern_matchers = [
                _ext_is("txt"),
                _ext_is("py"),
                _ext_is("xml"),
                _ext_is("ini"),
                _ext_is("pdb"),
                _ext_is("suo"),
                _ext_is("ilk"),
                _ext_is("idb"),
                _match("wconf"),
                _match("wscript"),
                _match("Thumbs.db"),
                ]


    def _is_ignored(path):
        global ignore_pattern

        for m in ignore_pattern_matchers:
            if m(path):
                return True

        return False

    def _get_commit_revision(wc):
      svn = SVN.get_instance()

      if os.path.isdir(wc):
          wc_dir = wc
      else:
          wc_dir = os.path.dirname(wc)

      try:
          svn_info = svn.info(wc_dir)
      except pysvn.ClientError, e:
          raise Errors.WafError(u'Src Directory %s is not svn working copy:%s' % (wc_dir,e))

      if not svn_info or not hasattr(svn_info, 'commit_revision'):
          raise Errors.WafError(u'svn info error in %s' % wc_dir)

      end   = svn_info.commit_revision
      limit = 1

      while (end.number - limit) > 0:
          svn_log = svn.log(svn_info.url,
                         end,
                         limit = limit,
                         discover_changed_paths = True)

          if len(svn_log) < 1:
              Logs.debug("Cannot get svn log from %s (revision=%d, limit=%d)" % (svn_info.url, end, limit))
              return None

          for log in svn_log:
              for changed_path in log['changed_paths']:
                  if _is_ignored(changed_path['path']):
                      Logs.debug(u"%s 忽略%r:%s" % (wc, log['revision'], changed_path['path'].decode('utf-8')))
                  else:
                      return log['revision'].number

          end   = pysvn.Revision(pysvn.opt_revision_kind.number,
                                 svn_log[-1]['revision'].number - 1)
          limit = limit * 2

      return -1

    revisions = [_get_commit_revision(i) for i in _make_list(wc)]

    return max(revisions)


class WCONF():
    u"""waf编译配置"""
    pass

def _load_wconf(wconf, check=False):
    def _check_url(url):
      import urlparse
      import httplib

      result = urlparse.urlparse(url)
      conn   = httplib.HTTPConnection(result.hostname, timeout=1)

      try:
        conn.request('HEAD', result.path)
      except BaseException, e:
        return (False, u"请求%s失败" % url)

      try:
          response = conn.getresponse()
      except BaseException, e:
        return (False, u"获取%s响应失败" % url)

      if response.status >= 400:
        return (False, u"连接%s失败 : %d , %s" % (url,
                                                  response.status,
                                                  response.reason))

      return (True, None)

    def _check_version(k,v):

        version = _parse_version(v).split('.')

        if(len(version) != 4):
            return (False, u"版本号应为4位")

        if all([i.isdigit() for i in version]):
            return (True, None)
        else:
            return (False, u"版本号格式错误, 应为4位.分隔的整数, 如x.x.x.x(x为整数)")

    def _check_rc(k, v):

        for rc in _make_list(v):
            l = glob.glob(rc)
            if not l:
                return (False, u"未找到匹配模式%s的文件" % rc)

            for f in l:
                if not os.path.isfile(f):
                    return (False, u"模式%s匹配的文件%s不存在" % (rc, f))

        return (True, None)


    def _check_exclude(k, v):
        type_match = lambda x  : type(x) in (types.UnicodeType, types.StringType, type(re.compile('')))

        if all( [ type_match(i) for i in _make_list(v) ] ):
            return (True, None)
        else:
            return (False, u"%s 应为str, unicode 或 _sre.SRE_Pattern" % (k))


    def _check_export_dir(k,v):
        if not os.path.isdir(v):
            Logs.debug(u"目录%s不存在" % v)

        return (True, None)

    def _check_upload_url(k,v):
        return _check_url(v)

    def _check_export(k,v):
        if not isinstance(v, types.DictType):
            return (False, u"%s应为dict类型" % k)

        for (cat, conf) in v.iteritems():
            if not isinstance( conf, types.ListType):
                return (False, u"类别 %s 的value(设置)应为list" % cat)

            if len(conf) < 2:
                return (False, u"类别 %s 的value(设置)非法, 应配置导出目录, [可选BASEDIR] 和文件列表" % cat)

            if isinstance(conf[1], types.DictType):
                if not conf[1].has_key('BASEDIR'):
                    return (False, u"类别 %s 的配置错误，无BASEDIR" % cat)

        return (True, None)

    def _check_hook(k,v):
        if not isinstance(v, types.DictType):
            return (False, u"%s应为dict类型" % k)

        for target, command in v.iteritems():
            if not hasattr(command, '__call__'):
                return (False, u"target %s 对应的value应为callable object" % k)

        return (True, None)


    # 读取配置文件，检查是否正确
    with open(wconf, "r") as f:

        d = {}

        try:
            exec f in d
        except Exception, e:
            raise Errors.WafError(u"读取配置文件%s失败: %r" % (wconf, e))

        required = True

        known_keys  = (('APPNAME',
                        lambda k,v : (isinstance(v, types.StringType), u"只允许字符串"),
                        required == False),

                       ('VERSION', _check_version, required == True),

                       ('SOLUTION',
                        lambda k,v : (os.path.isfile(v), u"文件%s不存在" % v),
                        required == True),

                       ('RC', _check_rc, required == True),

                       ('EXCLUDE', _check_exclude, required == True),

                       ('EXPORT_DIR', _check_export_dir, required == True),
                       ('UPLOAD_URL', _check_upload_url, required == True),

                       ('EXPORT', _check_export, required == True),
                       ('HOOK',   _check_hook,   required == False),
                       ('VERSION_PATH',
                        lambda k,v : (os.path.exists(v), u"路径%s不存在" % v),
                        required == False),
                      )

        for (key, checker, required) in known_keys:
            val = d.get(key)

            if check:
              if required:
                  if not val:
                      err = u"%s 文件未配置 %s" % (wconf, key)
                      Logs.error(err)

              if val and checker:
                  try:
                      (valid, msg) = checker(key, val)
                  except Exception, e:
                      err = u"%s: %s 配置错误: %s" % (wconf, key, e)
                      Logs.error(err)
                  else:
                      if not valid:
                          err = u"%s: %s 配置错误: %s" % (wconf, key, msg)
                          Logs.error(err)

            setattr(WCONF, key, val)

            if check:
              # 检查EXPORT_DIR是否与UPLOAD_URL匹配
              if WCONF.EXPORT_DIR and WCONF.UPLOAD_URL and os.path.exists(WCONF.EXPORT_DIR):
                  wc = _get_svn_wc_info(WCONF.EXPORT_DIR)

                  if wc.url != WCONF.UPLOAD_URL:
                      Logs.error(u"EXPORT_DIR(%s)应从UPLOAD_URL(%s)检出" %
                                                      (WCONF.EXPORT_DIR, WCONF.UPLOAD_URL))


if os.path.isfile(os.path.join(os.getcwd(), 'wconf')):
    try:
        _load_wconf(os.path.join(os.getcwd(), 'wconf'))
    except Exception,e:
        Logs.warn(e)
else:
    Logs.warn(u"当前目录(%s)下没有wconf文件" % os.getcwd())


### 选项
def options(opt):
    group = opt.add_option_group("custom options",
                        "Caution: use these options at your own risk.")

    group.add_option('-P', '--product-version',
                    action = 'store', dest = 'product_version',
                    help   = 'product version like 4.1.2.0 ')

    group.add_option('-M', '--message',
                    action = 'store', dest = 'message',
                    help   = 'message header to append in upload target')

    group.add_option('-T', '--targets',
                    action = 'store', dest = 'targets',
                    help   = 'files to be touched')

    group.add_option('-E', '--export-dir',
                    action = 'store', dest = 'export_dir',
                    help   = 'dir to export bin/pdb')

    group.add_option('-U', '--upload-url',
                    action = 'store', dest = 'upload_url',
                    help   = 'svn url to upload changed exe/pdb')

    group.add_option('-C', '--conf',
                    action = 'store', dest = 'conf',
                    help   = 'wconf file')

    group.add_option('--release',
                    action = "store", dest = 'release', metavar="Y", default="Y",
                    help   = 'build release version [default: %default]')

    group.add_option('--debug',
                    action = "store", dest = 'debug', metavar="N", default="N",
                    help   = 'build debug version [default: %default]')

    group.add_option('--revert',
                    action = "store_true", dest = 'revert',
                    help   = 'revert IT directory before export files')

    group.add_option('--purge',
                    action = "store_true", dest = 'purge',
                    help   = 'purge IT directory before conflict with upload url')

    group = opt.add_option_group("fastforward options",
                        "Caution: use these options while auto-building.")

    group.add_option('--no-update',
                    action = "store_false", dest = 'update', default=True,
                    help   = 'do not update IT directory')


### Targets
@hook
def configure(conf):
    call('version')

conf = config = configure

@hook
def version(ctx):
    def _replace_rc_with_patterns(line, patterns):
        for (pattern, version, sep) in patterns:
            (modified, substitue) = _replace_rc_with_version(line, pattern, version, sep)
            if modified:
                return substitue

        return None


    def _replace_rc_with_version(line, pattern, build_version, sep):
        """
        @ret : tuple (modified : bool, line : str)
        """

        match = re.search(pattern, line)

        # 不匹配
        if not match:
            return (False, None)

        header    = match.group()
        remainder = line[match.end():]

        match = re.search(r'([0-9,. ]*)(\D*)', remainder)

        #可能有引号等其它符号
        groups = match.groups()
        if len(groups) != 2:
            raise Errors.ConfigurationError(u"pattern(%s) must contain  one and only one group" % pattern)

        rc_version = re.split(r"[,\.]\s*", groups[0])

        if len(rc_version) != 4:
            raise Errors.ConfigurationError("""Check %s, version number format error!
                   Expect x.x.x.x, but found %s""" % (header, remainder))

        version = build_version.split('.')

        # 版本号一致

        if rc_version == version:
            return (False, None)

        return ( True, "%s%s%s" % (header, sep.join(version), groups[1]) )


    def _replace_h_with_patterns(line, patterns):
        for (pattern, version) in patterns:
            (modified, substitue) = _replace_h_with_version(line, pattern, version)
            if modified:
                return substitue

        return None


    def _replace_h_with_version(line, pattern, build_version):
        """
        @ret : tuple (modified : bool, line : str)
        """

        match = re.search(pattern, line)

        # 不匹配
        if not match:
            return (False, None)

        header    = match.group()
        remainder = line[match.end():]

        match = re.search(r'(\d+)', remainder)
        if not match:
            raise Errors.ConfigurationError(u"%s 格式错误，未找到版本号" % (line))

        rc_version = match.group()

        # 版本号一致

        if rc_version == build_version:
            return (False, None)

        return ( True, "%s%s\n" % (header, build_version) )


    def _substitute_rc(rc, file_version, product_version) :
        """
        @ret : tuple (changed : bool, contents : list[str])
        """
        bFoundVersionInfo = False
        bRcChanged        = False

        fileContents     = []

        with open(rc, 'r') as f:
            for line in f:
                fileContents.append(line)

                if line.lstrip().startswith('VS_VERSION_INFO'):
                    bFoundVersionInfo = True;
                    break

            if not bFoundVersionInfo:
                ctx.fatal(u"Error: %s中未找到VS_VERSION_INFO段" % rc)
                raise Errors.ConfigurationError(u"版本文件 %s 错误" % rc)

            patterns = [(r'(^.*FILEVERSION\s+)',    file_version,    r','),
                        (r'(^.*PRODUCTVERSION\s+)', product_version, r','),
                        (r'(^.*FileVersion\D+)',    file_version,    r'.'),
                        (r'(^.*ProductVersion\D+)', product_version, r'.')]

            for line in f:
                substitute = _replace_rc_with_patterns(line, patterns)
                if substitute:
                    fileContents.append(substitute)
                    bRcChanged = True
                else:
                    fileContents.append(line)

            if bRcChanged:
                return (True, fileContents)
            else:
                return (False, None)


    def _substitute_h(h, file_version, product_version) :
        """
        @ret : tuple (changed : bool, contents : list[str])
        """
        bFoundVersionInfo = False
        bRcChanged        = False

        file_version_parts    = file_version.split('.')
        product_version_parts = product_version.split('.')

        assert len(file_version_parts)    == 4
        assert len(product_version_parts) == 4

        fileContents     = []

        with open(h, 'r') as f:

            patterns = [(r'(^#define\s+BH5000_MAJOR_VERSION\s+)',    product_version_parts[0]),
                        (r'(^#define\s+BH5000_MINOR_VERSION\s+)',    product_version_parts[1]),
                        (r'(^#define\s+BH5000_MICRO_VERSION\s+)',    product_version_parts[2]),
                        (r'(^#define\s+BH5000_PATCH_VERSION\s+)',    product_version_parts[3]),
                        (r'(^#define\s+BH5000_SVN_VERSION\s+)',      file_version_parts[3]) ]

            for line in f:
                if not line.startswith("#define"):
                    fileContents.append(line)
                    continue

                substitute = _replace_h_with_patterns(line, patterns)
                if substitute:
                    fileContents.append(substitute)
                    bRcChanged = True
                else:
                    fileContents.append(line)

            if bRcChanged:
                return (True, fileContents)
            else:
                return (False, None)

    def _substitute_cs(cs, file_version, product_version) :
        """
        @ret : tuple (changed : bool, contents : list[str])
        """
        bFoundVersionInfo = False
        bRcChanged        = False

        file_version_parts    = file_version.split('.')
        product_version_parts = product_version.split('.')

        assert len(file_version_parts)    == 4
        assert len(product_version_parts) == 4

        fileContents     = []

        with open(cs, 'r') as f:
            for line in f:

                if line.lstrip().startswith('[assembly: AssemblyVersion'):
                    fileContents.append('''[assembly: AssemblyVersion("%s")]\n''' % product_version)
                    bRcChanged = True
                elif line.lstrip().startswith('[assembly: AssemblyInformationalVersion'):
                    fileContents.append('''[assembly: AssemblyInformationalVersion("%s")]\n''' % product_version)
                    bRcChanged = True
                elif line.lstrip().startswith('[assembly: AssemblyFileVersion'):
                    fileContents.append('''[assembly: AssemblyFileVersion("%s")]\n''' % file_version)
                    bRcChanged = True
                else:
                    fileContents.append(line)

            if bRcChanged:
                return (True, fileContents)
            else:
                return (False, None)


    def _update_build_version(product_version):
        # 4位版本号
        if len(product_version.split(".")) != 4:
            raise ValueError, u"版本号应为x.x.x.x， 而不是%s" % (product_version)

        rcConf = _make_list(WCONF.RC)

        rcList = []
        for rc in rcConf:
            rcList.extend(glob.glob(rc))

        if not rcList:
            ctx.fatal(u'Error: RC files not found')

        svn = SVN.get_instance()

        #print 'Product Version =', _color_as_info(product_version)

        version = product_version.split('.')

        for conf in rcList:
            abspath  = os.path.abspath(conf)
            dirname  = os.path.dirname(abspath)
            basename = os.path.basename(abspath)

            substituter = _substitute_rc

            # rc2 文件使用上一级目录获取最后一次修改版本
            if basename.endswith('.rc'):
                substituter = _substitute_rc
            elif basename.endswith('.rc2'):
                dirname = os.path.dirname(dirname)
                substituter = _substitute_rc
            elif basename.endswith('.h'):
                dirname = os.getcwd()
                substituter = _substitute_h
            elif basename.endswith('.cs'):
                dirname = os.path.dirname(dirname)
                substituter = _substitute_cs
            else:
                raise Errors.WafError(u"不支持的资源文件: %s" % conf)

            if WCONF.VERSION_PATH:
                svn_revision = _get_svn_changed_revision(WCONF.VERSION_PATH)
            else:
                svn_revision = _get_svn_changed_revision(abspath)

            if svn_revision <= 0:
                raise Errors.WafError(u"修改%s失败, 无法读取目录%s对应的SVN版本" % (conf, dirname))

            version[-1] = str(svn_revision)
            file_version = '.'.join(version)

            #print 'File    Version =', _color_as_info(file_version)

            (bRcChanged, fileContents) = substituter(abspath, file_version, product_version)

            if bRcChanged:
                with open(conf, 'w') as f:
                    f.writelines(fileContents)
                    Logs.info(u'文件 %s 版本号更新 %s : %s' %  (_local_to_unicode(abspath), file_version, product_version))
            else:
                Logs.info(u'文件 %s 版本号早已更新 %s : %s' %  (_local_to_unicode(abspath), file_version, product_version))


    ## 修改版本号

    if ctx.options.conf:
        conf = ctx.options.conf
        _load_wconf(os.path.join(ctx.path.abspath(), conf))

    if ctx.options.product_version:
        product_version = _parse_version(ctx.options.product_version)
        print '  force   :', _color_as_important(product_version)
    else:
        product_version = _parse_version(WCONF.VERSION)

    if ctx.options.product_version:
        if ctx.options.product_version == 'manual':
            product_version = _parse_rc_version(WCONF.RC)
            print '  manual   :', _color_as_important(product_version)
        else:
            product_version = _parse_version(ctx.options.product_version)
            print '  force   :', _color_as_important(product_version)
    else:
        if WCONF.VERSION == 'manual':
            product_version = _parse_rc_version(WCONF.RC)
            print '  manual   :', _color_as_important(product_version)
        else:
            product_version = _parse_version(WCONF.VERSION)

    Logs.info("检查并更新版本号 %s ..." % product_version)

    _update_build_version(product_version)

@hook
def build(bld):
    if bld.options.conf:
        conf = bld.options.conf
        _load_wconf(os.path.join(bld.path.abspath(), conf))

    if bld.cmd == 'clean':
        if bld.options.release[0].upper() == 'Y':
            command = _create_build_command(os.path.join(bld.path.abspath(), WCONF.SOLUTION),
                                            instruction   = "clean",
                                            configuration = "release")
            bld.exec_command(command)

        if bld.options.debug[0].upper() == 'Y':
            command = _create_build_command(os.path.join(bld.path.abspath(), WCONF.SOLUTION),
                                            instruction   = "clean",
                                            configuration = "debug")
            bld.exec_command(command)
    elif bld.cmd == 'build':
        if bld.options.release[0].upper() == 'Y':
            command = _create_build_command(os.path.join(bld.path.abspath(), WCONF.SOLUTION),
                                            instruction   = "build",
                                            configuration = "release")
            bld.exec_command(command, shell=True)

        if bld.options.debug[0].upper() == 'Y':
            command = _create_build_command(os.path.join(bld.path.abspath(), WCONF.SOLUTION),
                                            instruction   = "build",
                                            configuration = "debug")
            bld.exec_command(command)
    elif bld.cmd == 'install':
        raise Errors.WafError("install command not implemented.")
    else:
        raise Errors.WafError("command %s not implemented." % bld.cmd)

@hook
def rebuild(ctx):
    if ctx.options.conf:
        conf = ctx.options.conf
        _load_wconf(os.path.join(ctx.path.abspath(), conf))

    if ctx.options.release[0].upper() == 'Y':
        command = _create_build_command(os.path.join(ctx.path.abspath(), WCONF.SOLUTION),
                                        instruction   = "rebuild",
                                        configuration = "release")
        ctx.exec_command(command)

    if ctx.options.debug[0].upper() == 'Y':
        command = _create_build_command(os.path.join(ctx.path.abspath(), WCONF.SOLUTION),
                                        instruction   = "rebuild",
                                        configuration = "debug")

        ctx.exec_command(command)

@hook
def export(ctx):
    if ctx.options.conf:
        conf = ctx.options.conf
        _load_wconf(os.path.join(ctx.path.abspath(), conf))

    if ctx.options.export_dir:
        exportDir = os.path.abspath(ctx.options.export_dir)
    else:
        exportDir = os.path.abspath(WCONF.EXPORT_DIR)

    if ctx.options.upload_url:
        uploadURL = ctx.options.upload_url
    else:
        uploadURL = WCONF.UPLOAD_URL

    svn = SVN.get_instance()

    if not os.path.isdir(exportDir):
        Logs.info(u"svn checkout %s to %s" % (uploadURL, exportDir))
        svn.checkout(uploadURL, exportDir)
    else:
        try:
            wc = svn.info(exportDir)
        except pysvn.ClientError, e:
            Logs.warn(u"Can't get working directory status : %s" % e)
        else:
            if wc.url == uploadURL:

                if ctx.options.revert:
                    Logs.warn(u"svn revert %s" % exportDir)
                    try:
                        svn.revert(exportDir, recurse=True)
                    except pysvn.ClientError, e:
                        raise Errors.WafError(u'目录%s恢复失败' % exportDir)

                if ctx.options.update:
                  Logs.info(u"svn update %s" % exportDir)
                  try:
                      wc = svn.update(exportDir)
                  except pysvn.ClientError, e:
                      raise Errors.WafError(u'目录%s更新失败' % exportDir)
            else:
                Logs.error(u"exportDir(%s) supposed to be checkout from %s instead of %s" \
                        % (os.path.abspath(exportDir), uploadURL, wc.url))

                if ctx.options.purge:
                    Logs.warn(u"purge %s" % exportDir)
                    shutil.rmtree(exportDir)

                    Logs.info(u"svn checkout %s to %s" % (uploadURL, exportDir))
                    svn.checkout(uploadURL, exportDir)

    for (category, conf) in WCONF.EXPORT.iteritems():
        Logs.info(u'* Copy %s ...' % category)

        dest = conf.pop(0)

        # EXPORT_DIR需替换为exportDir
        if dest.startswith(WCONF.EXPORT_DIR):
            dest = os.path.join(exportDir, dest[len(WCONF.EXPORT_DIR) + 1:])
        else:
            raise Errors.WafError(u"拷贝目标目录应以EXPORT_DIR开始")

        # 设置BASEDIR时需拷贝目录结构
        basedir = None
        if type(conf[0]) == types.DictType:
            basedir = conf.pop(0).pop('BASEDIR', None)

        exports = conf.pop(0)

        for e in exports:

            if basedir:
                e = os.path.join(basedir, e)

            if os.path.isfile(e):
                _copy_file(e, dest, basedir)
                continue

            items = glob.glob(e)

            if not items:
                raise Errors.WafError(u"%r不存在" % e)

            for i in items:
                if os.path.isfile(i):
                    if not _exclude(i):
                        _copy_file(i, dest, basedir)
                elif os.path.isdir(i):
                    for root, dirs, files in os.walk(i):
                        for d in dirs:
                            if _exclude(d):
                                dirs.remove(d)

                        for f in files:
                            if _exclude(f):
                                continue
                            else:
                                _copy_file(os.path.join(root, f), dest, basedir)
                else:
                    raise Errors.WafError(u'%s is not file or directory' % i)

@hook
def upload(ctx):
    if ctx.options.conf:
        conf = ctx.options.conf
        _load_wconf(os.path.join(ctx.path.abspath(), conf))

    if ctx.options.export_dir:
        exportDir = os.path.abspath(ctx.options.export_dir)
    else:
        exportDir = os.path.abspath(WCONF.EXPORT_DIR)

    if ctx.options.upload_url:
        uploadURL = ctx.options.upload_url
    else:
        uploadURL = WCONF.UPLOAD_URL


    if not os.path.isdir(exportDir):
        raise Errors.WafError(u'Export directory(%s) does not exist' % exportDir)

    svn = SVN.get_instance()

    # src working copy
    try:
        src_wc = svn.info(ctx.path.abspath())
    except pysvn.ClientError, e:
        raise Errors.WafError(u'Src Directory %s is not svn working copy' % ctx.path.abspath())

    try:
        log = svn.log(src_wc.url, src_wc.commit_revision, src_wc.commit_revision)
    except pysvn.ClientError, e:
        raise Errors.WafError(u'Cannot get log %s@r%r' % (src_wc.url, src_wc.commit_revision))

    # export working copy
    try:
        export_wc = svn.info(exportDir)
    except pysvn.ClientError, e:
        raise Errors.WafError(u'Directory %s is not svn working copy' % exportDir)

    if export_wc.url.upper() != uploadURL.upper():
        raise Errors.WafError(u'Export Directory(%s) shoud be checkout from %s instead of %s') \
                              % (exportDir, uploadURL, export_wc.url)

    try:
        svn.update(exportDir)
    except pysvn.ClientError, e:
        raise Errors.WafError(u'Cannot update working copy %s' % exportDir)

    try:
        status = svn.status(exportDir,
                       get_all=False,
                       ignore=True,
                       ignore_externals=True)
    except pysvn.ClientError:
        raise Errors.WafError(u'Cannot status working copy %s' % exportDir)


    if len(status) < 1:
        raise Errors.WafError(u"Noting to upload.")

    if ctx.options.message:
        msg = u"%s %s@r%d" % (ctx.options.message, src_wc.url, src_wc.commit_revision.number)
    else:
        import socket
        (_, _, ipaddr_list) = socket.gethostbyname_ex(socket.gethostname())

        ip = []
        for i in ipaddr_list:
            if i.startswith('10.') or i.startswith('127.'):
                pass
            else:
                ip.append(i)

        msg = u"%s\ncommitted by %s, built by bot %s using %s@r%d" % (log[0].message.decode('utf8'),
                                              src_wc.commit_author,
                                              u"/".join(ip),
                                              src_wc.url,
                                              src_wc.commit_revision.number)

    rev = svn.checkin(exportDir, msg)

    if rev:
        Logs.info(u"upload %s@r%d --> %s@r%d" % (src_wc.url, src_wc.commit_revision.number, export_wc.url, rev.number))
    else:
        raise Errors.WafError(u"未提交任何东西至%s" % export_wc.url)


def info(ctx):
    if ctx.options.conf:
        conf = ctx.options.conf
        _load_wconf(os.path.join(ctx.path.abspath(), conf))
    else:
        conf = 'wconf'

    print u'Working Path Info:'
    print u'  waf      :', os.path.abspath(sys.argv[0])
    print u'  wscript  :', os.path.abspath(__name__)
    print u'  wconf    :', os.path.abspath(conf)

    assert(os.getcwd() == ctx.path.abspath())
    print u'  cwd      :', ctx.path.abspath()
    print

    print u'wconf Settings:'
    print u'  APPNAME  :', WCONF.APPNAME
    print u'  VERSION  :', WCONF.VERSION
    print u'  SOLUTION :', os.path.abspath(WCONF.SOLUTION)
    print u'  RC       :', map(os.path.abspath, _make_list(WCONF.RC))
    print

    print u'  EXPORT_DIR   :', os.path.abspath(WCONF.EXPORT_DIR)
    print u'  UPLOAD_URL   :', WCONF.UPLOAD_URL
    print

    # svn working diretory status
    svn = SVN.get_instance()

    try:
        wc = svn.info(ctx.path.abspath())
    except pysvn.ClientError, e:
        Logs.error(u"Can't get working directory status : %s" % e)
        return False

    try:
        log = svn.log(wc.url, wc.commit_revision, wc.commit_revision)
    except pysvn.ClientError, e:
        Logs.error(u"Can't get working directory status : %s" % e)
        return False

    print u'SVN Working Directory Info:'
    print u'  url      :', wc.url
    print u'  revision :', wc.commit_revision.number
    print u'  author   :', wc.commit_author
    print u'  log      :', _utf8_to_local(log[0].message)
    print

    print u'Version'
    print u'  conf    :', _parse_version(WCONF.VERSION)


### SVN 命令
def update(ctx):
    svn = SVN.get_instance()
    ret = svn.update(ctx.path.abspath())

    revision = SVN.get_update_revision()

    if revision:
        print u'SVN工作目录 (%s) 更新至 r%d' % (ctx.path.abspath(), revision.number)
    else:
        ctx.fatal('SVN工作目录更新失败')

up = update

def revert(ctx):
    svn = SVN.get_instance()
    svn.revert(ctx.path.abspath(), recurse=True)


def commit(ctx):

    TortoiseProc = _get_tortoiseproc()

    if TortoiseProc:
        command = " ".join([
                        '"%s"' % TortoiseProc,
                        "/closeonend:1",
                        "/command:commit",
                        "/path:%s" % ctx.path.abspath()])


        ctx.exec_command(command)

ci = commit


def status(ctx):

    TortoiseProc = _get_tortoiseproc()

    if TortoiseProc:
        command = " ".join([
                        '"%s"' % TortoiseProc,
                        "/closeonend:1",
                        "/command:repostatus",
                        "/path:%s" % ctx.path.abspath()])


        ctx.exec_command(command)

st = status


def cleanup(ctx):
    svn = SVN.get_instance()
    svn.cleanup(ctx.path.abspath())


def revisiongraph(ctx):

    TortoiseProc = _get_tortoiseproc()

    if TortoiseProc:
        command = " ".join([
                        '"%s"' % TortoiseProc,
                        "/closeonend:1",
                        "/command:revisiongraph",
                        "/path:%s" % ctx.path.abspath()])


        ctx.exec_command(command)

rg = revisiongraph

def del_svnprop_mergeinfo(ctx):
    class SvnPropRemover(object):

        """
        只保留根目录属性，其它全删除
        """

        def __init__(self, prop):
            self.svn  = SVN.get_instance()
            self.prop = prop

        def del_mergeInfo(self, path_list):

            for p in path_list:
                self.svn.propdel(self.prop, p)
                print 'svn:mergeinfo deleted in', p


        def get_subPathList_containing_mergeInfo(self, path):
            merge_info_list = self.svn.propget(self.prop,
                                          path,
                                          depth = pysvn.depth.infinity)

            if not merge_info_list:
                return []

            path_list = merge_info_list.keys()

            return [os.path.abspath(p)
                            for p in path_list
                                if p
                                if not SvnPropRemover.is_same_path(path, p)]


        def del_mergeInfo_in_subPathList(self, path):

            self.del_mergeInfo(self.get_subPathList_containing_mergeInfo(path))


        @staticmethod
        def is_same_path(p1, p2):
            norm_path  = lambda p : os.path.normcase(os.path.abspath(p))
            if "nt" == os.name:
                return norm_path(p1) == norm_path(p2)
            else:
                return os.path.samefile(p1, p2)

    mergeInfoRemover = SvnPropRemover("svn:mergeinfo")
    mergeInfoRemover.del_mergeInfo_in_subPathList(ctx.path.abspath())

propdel = del_svnprop_mergeinfo

### 系统命令
def touch(ctx):
    def _touch(fname, times = None):
        with file(fname, 'a'):
            os.utime(fname, times)

    if ctx.options.targets:
        targets = _make_list(ctx.options.targets.split(" "))
        for t in targets:
            _touch(t)

def explore(ctx):
    if ctx.options.conf:
        conf = ctx.options.conf
        _load_wconf(os.path.join(ctx.path.abspath(), conf))

    if os.path.exists(WCONF.SOLUTION):
        command = u'"%s" /e,/select,"%s"' % (_get_explorer(), WCONF.SOLUTION)
        ctx.exec_command(command)

### 测试
@hook
def test(ctx):
    #if ctx.options.conf:
    #    conf = ctx.options.conf
    #    _load_wconf(os.path.join(ctx.path.abspath(), conf))
    #
    #def test_wconf():
    #    from pprint import pprint
    #    for p  in dir(WCONF):
    #        if p.startswith("__"):
    #            continue
    #
    #        print "[%s]" % p
    #        print ' ' * 4,
    #        pprint(eval("WCONF.%s" % p), indent = 4)
    #
    #print _get_devenv()
    #print _get_explorer()
    #print _get_msiexec()
    #print _get_python()

    print "Running test ..."


def checkconf(ctx):

  if ctx.options.conf:
    ret = _load_wconf(os.path.join(ctx.path.abspath(), ctx.options.conf),
                      check = True)
  else:
    ret = _load_wconf(os.path.join(os.getcwd(), 'wconf'),
                      check = True)


### 帮助
def help(ctx):
    Logs.info(u"使用方式")
    print u"Example 1: 一键编译"
    print u"  1) waf configure rebuild export"
    print
    print u"Example 2: 指定产品版本号"
    print u"  0) waf configure: (第一次执行一次即可)"
    print u"  1) waf configure或version -P 4.2.1.0 或者 waf configure或version --product-version 4.2.1.0"
    print u"  2) waf rebuild"
    print u"  3) waf export"
    print

    call('targets')


def targets(ctx):
    Logs.info(u"常用命令:")
    print u"  configure : 配置"
    print u"  version   : 版本化"
    print u"  clean     : 清理"
    print u"  build     : 编译"
    print u"  rebuild   : 重新编译"
    print u"  export    : 拷贝编译结果至IT目录"
    print u"  upload    : 上传编译结果至SVN"
    print

    Logs.info(u"SVN快捷命令")
    print u"  update/up : 执行svn update"
    print u"  cleanup   : 执行svn cleanup"
    print u"  revert    : 执行svn revert"
    print u"  status/st : 执行TortoiseSVN Check for modification"
    print u"  commit/ci : 执行TortoiseSVN Commit"
    print u"  revisiongraph/rg : 执行TortoiseSVN Revision Graph"
    print u"  propdel   : 递归删除子目录svn:mergeinfo属性, 相当于\n",
    print u"     svn ls | sed -e s%/%% | xargs -I % svn propdel svn:mergeinfo --depth=infinity %"
    print

    Logs.info(u"其它")
    print u"  touch     : 创建文件或更新文件修改时间"
    print u"  explore   : 资源浏览器打开当前目录"
