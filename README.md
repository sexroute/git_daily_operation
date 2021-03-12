# Git daily operation
## 1 Init repositories operation

### 1.1 or create a new repository on the command line
echo "# git_daily_operation" >> README.md  
git init  
git add README.md   
git commit -m "first commit"  
git branch -M main  
git remote add origin https://github.com/sexroute/git_daily_operation.git  
git push -u origin main  

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
### 3.1 How to remove a too large file in a committed repository
a.delete already commited large file and keep local one  
local repository directory>**git rm --cached <somefile.ext>**  
then commit:  
local repository directory>**git commit**  
b. clone remote or local repository to local directory in mirror mode: name like a.git  
parent directory of local repository>**git clone --mirror \<local git repository path>**  
c. download bfg-1.13.0.jar  (from https://rtyley.github.io/bfg-repo-cleaner/)  
d. go to the parent ditectory of the mirrored cloned repository, exec  
parent directory of local repository>**java -jar bfg-1.13.0.jar --strip-blobs-bigger-than 100M**  
If you are prompted:  
>Scanning packfile for large blobs completed in 16 ms.  
>Warning : no large blobs matching criteria found in packfiles does the repo need to be packed?  

Enter path cloned in step b then execute  
cloned mirror directory>**git gc**  
Then go to the parent ditectory of the mirrored cloned repository, execute  
parent directory of local repository>**java -jar bfg-1.13.0.jar --strip-blobs-bigger-than 100M <path cloned in step b>**  
e.enter the cloned directory and exec   
cloned mirror directory>**git push**  
If you are prompted for **"bare"** related messages, commit to the local source library to allow non-bare  
local repository directory>**git config --bool core.bare true**   
go to he clone directory and exec push again  
cloned mirror directory>**git push**  

f. If it is a local library, you need to restore the local library's BARE no-commit mode  
local repository directory>**git config --bool core.bare false**  
### 3.2 install git lfs
git lfs install  
git lfs track "\*.psd"  
git lfs track "\*.doc"  
git lfs track "\*.docx"  
git lfs track "\*.xls"  
git lfs track "\*.pdf"  
git lfs track "\*.xlsx"  
git lfs track "\*.ppt"  
git lfs track "\*.pptx"  
git lfs track "\*.zip"  
git lfs track "\*.exe"  
git lfs track "\*.dll"  
git lfs track "\*.wav"  
git add .gitattributes  
git commit -m "lfs" -a  
git push  

### 3.3 git set and unset proxy
git config --global http.proxy http://127.0.0.1:1080  
git config --global https.proxy http://127.0.0.1:1080  
git config --global --unset http.proxy  
git config --global --unset https.proxy  
git config --global http.sslverify false  

## git tag  
git tag -a v1.4 -m "my version 1.4"  
git push origin --tags  

## git save username and password
git config --global credential.helper store  
if prompted " fatal: $HOME not set"  
git config --system user.email "your name here"  
git config --system user.name "your email@example.com"  


