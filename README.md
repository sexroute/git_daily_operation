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
 
### 2.2 checkout remote branch
git checkout -b origin/release/caigou_v1.0     
 
### 2.3 display local branch and tracking remote branch
git branch -vv
 
### 2.4 push local branch to remote branch and track
git push -u origin/release/caigou_v1.0

## 3 git large file
### 3.1 delete already commited large file
a.ʹ�� git ����ɾ��Ҫɾ���Ĵ��ļ����磺100M���ϣ�Ȼ���ύ������  
b.ʹ�� git clone --mirror http://172.16.10.13/chenmingjian/test-project.git(Զ�̣� ���߱���GIT ·��  
c.���� bfg-1.13.0.jar  https://rtyley.github.io/bfg-repo-cleaner/  
d.ִ�� java -jar bfg-1.13.0.jar --strip-blobs-bigger-than 100M <�ڶ�����¡������·��>  
�����ʾ:  
Scanning packfile for large blobs completed in 16 ms.  
Warning : no large blobs matching criteria found in packfiles - does the repo need to be packed?  
����<�ڶ�����¡������·��> ִ�� 
git gc
Ȼ����ִ�� java -jar bfg-1.13.0.jar --strip-blobs-bigger-than 100M <�ڶ�����¡������·��>  
e.ִ�� git push  
�����ʾbare��ص���Ϣ����Ա��ص�Դ��ִ�������bare���ύ����ִ��push  
f.����Ǳ��ؿ⣬����Ҫ�ָ����ؿ��bare��ֹ�ύģʽ

### 3.2 ��װ git lfs
git lfs install  
git lfs track "*.psd"
git add .gitattributes
git commit -m "<some message>" -a
git push