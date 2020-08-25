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
a. **git rm --cached <somefile.ext>** For example :100M above, then submit: **git commit**  
b. use  
**git clone --mirror <http://172.16.10.13/chenmingjian/test-project.git(remote) or  local git repository path>**  
c. download bfg-1.13.0.jar  (from https://rtyley.github.io/bfg-repo-cleaner/)  
d. exec  
**java -jar bfg-1.13.0.jar --strip-blobs-bigger-than 100M <The path cloned in step b>**  
If you are prompted:  
>Scanning packfile for large blobs completed in 16 ms.  
>Warning : no large blobs matching criteria found in packfiles does the repo need to be packed?  

Enter path cloned in step b then execute  
**git gc**  
Then execute  
**java -jar bfg-1.13.0.jar --strip-blobs-bigger-than 100M <path cloned in step b>**  
e. exec **git push**  
If you are prompted for "bare" related messages, commit to the local source library to allow non-bare, and then exec push as follow 
**git config --bool core.bare true  
**git push 

f. If it is a local library, you need to restore the local library's BARE no-commit mode 
**git config --bool core.bare false 
### 3.2 install git lfs
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
