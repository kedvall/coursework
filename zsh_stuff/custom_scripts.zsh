# You can put files here to add functionality separated per file, which
# will be ignored by git.
# Files on the custom/ directory will be automatically loaded by the init
# script, in alphabetical order.

# For example: add yourself some shortcuts to projects you often work on.
#
# brainstormr=~/Projects/development/planetargon/brainstormr
# cd $brainstormr
#

alias mp3='cd /mnt/c/Users/kedvall/dev/ece391/ece391_share/work/mp3'
cd '/mnt/c/Users/kedvall/dev/ece391/ece391_share/work/mp3'
alias dev='cd /mnt/c/Users/kedvall/dev/'
alias down='cd /mnt/c/Users/kedvall/Downloads/'

alias gc="git add --all ; git commit -m'"
alias gs='git status'

alias gdb='gdb -q'
alias ez='vim ~/.oh-my-zsh/custom/custom_scripts.zsh ; . ~/.oh-my-zsh/custom/custom_scripts.zsh'

# Funtion to take parameters and open text editor with said parameters
openEditor() {
    subl.exe "$@" ;
}

# Simple script to go "up" specified num of directories. Defaults to 1
up() { cd $(eval printf '../'%.0s {1..$1}) && pwd; }

alias sub='openEditor'
