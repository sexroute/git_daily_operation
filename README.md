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
	
git filter-branch --tree-filter 'rm -rf nft_analyze/0x60e4d786628fea6478f785a6d7e704777c86a7c6/0x60e4d786628fea6478f785a6d7e704777c86a7c6_assets.json' HEAD. 

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
git lfs track "\*.pdb"  
git lfs track "\*.rar"  
git lfs track "\*.msi"  
git lfs track "\*.lib"  
git lfs track "\*.dat"  
git lfs track "\*.gz"  
git lfs track "\*.tar"  
git lfs track "\*.so"  
git lfs track "\*.json"  
git lfs track "\*.csv"  
git lfs track "\*.txt"  
git lfs track "\*.png"  
git lfs track "\*.jpeg"  
git lfs track "\*.jpg"  
git lfs track "\*.json"  
git lfs track "\*.csv"  
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
## delete local tag. 
git tag -d  "tag_name"
  

## git save username and password
git config --global credential.helper store  
if prompted " fatal: $HOME not set"  
git config --system user.email "your name here"  
git config --system user.name "your email@example.com"  
	
## git clear saved username and password. 
git config --system --unset credential.helper   
git config --global --unset credential.helper  
git config --unset credential.helper  
git pull  
git config --local credential.helper cache  
git config --system credential.helper cache  
git config --global credential.helper cache  
git pull. 


## git reset like remote original/master (depend on the branch name)
git fetch origin  
git reset --hard origin  

## 4.Checkout github pull requests locally
#### 4.1 Locate the section for your github remote in the .git/config file. It looks like this:  
[remote "origin"]
	fetch = +refs/heads/*:refs/remotes/origin/*  
	url = git@github.com:joyent/node.git  
#### 4.2 Now add the line fetch = +refs/pull/*/head:refs/remotes/origin/pr/* to this section. Obviously, change the github url to match your project's URL. It ends up looking like this:  
  [remote "origin"]  
	fetch = +refs/heads/\*:refs/remotes/origin/\*  
	url = git@github.com:joyent/node.git  
	fetch = +refs/pull/\*/head:refs/remotes/origin/pr/\*  
#### 4.3 Now fetch all the pull requests:  
  $ git fetch origin  
    From github.com:joyent/node  
    * [new ref]         refs/pull/1000/head -> origin/pr/1000  
    * [new ref]         refs/pull/1002/head -> origin/pr/1002  
    * [new ref]         refs/pull/1004/head -> origin/pr/1004  
    * [new ref]         refs/pull/1009/head -> origin/pr/1009  
  ...  
#### 4.4 To check out a particular pull request:  
  $ git checkout pr/999  
  Branch pr/999 set up to track remote branch pr/999 from origin.  
  Switched to a new branch 'pr/999'  
  
## 5 clone use ssh   
	1. gen ssh key   
	ssh-keygen -o -t rsa -C "xxx@github.com"   
	Enter file in which to save the key (C:\Users\xxx/.ssh/id_rsa): C:\Users\xxx/.ssh/xxx_rsa  
	2.clone with specified ssh key   
	eval `ssh-agent -s`   
	ssh-agent $(ssh-add C:\Users\xxx/.ssh/xxx_rsa; git clone git@github.com:user/project.git)   
	3.change repo ssh key 
	git config core.sshCommand "ssh -i ~/.ssh/xxx_rsa -F /dev/null"
	



  
  

