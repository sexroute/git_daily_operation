# Git daily operation
## 1 Init repositories operation

### 1.1 or create a new repository on the command line
echo "# git_daily_operation" >> README.md  
git init  
git add README.md  
git commit -m "first commit"  
git remote add origin https://github.com/sexroute/git_daily_operation.git  
git push -u origin master  

### 1.2 or push an existing repository from the command line
git remote add origin https://github.com/sexroute/git_daily_operation.git
git push -u origin master    

### 1.3 or import code from another repository
You can initialize this repository with code from a Subversion, Mercurial, or TFS project.   

### 1.4 or Change your remote's URL from SSH to HTTPS with the git remote set-url command.
git remote set-url origin https://github.com/USERNAME/REPOSITORY.git

### 1.5 or check remote url
git remote -v

## 2 branch opertiaon  
  
### 2.1 display all local and remote branches
git branch -a  

dev  
master  
remotes/origin/HEAD -> origin/master  
remotes/origin/master  
remotes/origin/release/caigou_v1.0  

### 2.1.1 git fetch all branch data
git fetch  
### 2.2 checkout remote branch
git checkout -b caigou_v1.0 origin/release/caigou_v1.0     
 
### 2.3 display local branch and tracking remote branch
git branch -vv
 
### 2.4 push local branch to remote branch and track
git push -u origin/release/caigou_v1.0

## 3 git large file
### 3.1 delete already commited large file
a.使用 git 命令删除要删除的大文件，如：100M以上，然后提交  
b.使用 git clone --mirror http://172.16.10.13/chenmingjian/test-project.git(远程） 或者本地GIT 路径  
c.下载 bfg-1.13.0.jar  https://rtyley.github.io/bfg-repo-cleaner/  
d.执行 java -jar bfg-1.13.0.jar --strip-blobs-bigger-than 100M <第二部克隆出来的路径>  
如果提示:  
Scanning packfile for large blobs completed in 16 ms.  
Warning : no large blobs matching criteria found in packfiles - does the repo need to be packed?  
进入<第二部克隆出来的路径> 执行 
git gc
然后再执行 java -jar bfg-1.13.0.jar --strip-blobs-bigger-than 100M <第二部克隆出来的路径>  
e.执行 git push  
如果提示bare相关的消息，则对本地的源库执行允许非bare的提交，再执行push  
git config --bool core.bare true  
git push 

f.如果是本地库，则需要恢复本地库的bare禁止提交模式 
git config --bool core.bare false 
### 3.2 安装 git lfs
git lfs install  
git lfs track "*.psd"  
git lfs track "*.doc"  
git lfs track "*.docx"  
git lfs track "*.xls"  
git lfs track "*.pdf"  
git lfs track "*.xlsx"  
git lfs track "*.ppt"  
git lfs track "*.pptx"  
git lfs track "*.zip"  
git add .gitattributes  
git commit -m "lfs" -a  
git push  
