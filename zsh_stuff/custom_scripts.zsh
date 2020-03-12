# You can put files here to add functionality separated per file, which
# will be ignored by git.
# Files on the custom/ directory will be automatically loaded by the init
# script, in alphabetical order.

# For example: add yourself some shortcuts to projects you often work on.
#
# brainstormr=~/Projects/development/planetargon/brainstormr
# cd $brainstormr
#

cd '/mnt/c/Users/kedvall/dev/'
alias dev='cd /mnt/c/Users/kedvall/dev/'
alias down='cd /mnt/c/Users/kedvall/Downloads/'
alias exp='explorer.exe .'
alias lib='cd /mnt/c/Users/kedvall/Documents/Arduino/libraries/'

alias gc="git add --all ; git commit -m'"
alias gs='git status'
alias gp="git push"
alias gap="git all push"
alias gpl="git pull"
alias gapl="git all pull"
alias gpt="git push --tags"
alias gt="git tag -a"

alias gdb='gdb -q'
alias ez='vim ~/.oh-my-zsh/custom/custom_scripts.zsh ; . ~/.oh-my-zsh/custom/custom_scripts.zsh'
alias lf='ssh kedvall@192.168.31.100'
alias car='ssh -t cartesian@192.168.31.225 "cd cartesian_ui/ ; bash"'
alias kp='ssh kedvall@192.168.31.237'
alias rsub='ssh -t -R 52699:localhost:52699 cartesian@192.168.31.225 "cd cartesian_ui/ ; bash"'
alias rsubkp='ssh -t -R 52699:localhost:52699 kedvall@192.168.31.237 "cd dev/cartesian_ui/ ; bash"'
alias unifi='ssh LifeFoundry@192.168.31.1'

# ADD THIS for git 'all' commands
#git config --global alias.all '!f() { ls -R -d */.git | xargs -I{} bash -c "echo {} && git -C {}/../ $1"; }; f'

# Add cross compiler
export PATH=~/barebones-toolchain/cross/x86_64/bin/:$PATH

# Funtion to take parameters and open text editor with said parameters
openEditor() {
    subl.exe "$@" ;
}

# Simple script to go "up" specified num of directories. Defaults to 1
up() { cd $(eval printf '../'%.0s {1..$1}) && pwd; }

alias sub='openEditor'
