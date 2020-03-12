# vim:ft=zsh ts=2 sw=2 sts=2

rvm_current() {
  rvm current 2>/dev/null
}

rbenv_version() {
  rbenv version 2>/dev/null | awk '{print $1}'
}

PROMPT='
%{$FG[045]%}${PWD/#$HOME/~}%{$reset_color%}$(git_prompt_info) %{$FG[202]%}%*%{$reset_color%}
$ '

# Must use Powerline font, for \uE0A0 to render.
ZSH_THEME_GIT_PROMPT_PREFIX=" on %{$FG[046]%}\uE0A0 "
ZSH_THEME_GIT_PROMPT_SUFFIX="%{$reset_color%}"
ZSH_THEME_GIT_PROMPT_DIRTY="%{$FG[196]%}\u2718 "
ZSH_THEME_GIT_PROMPT_UNTRACKED="%{$FG[196]%}? "
ZSH_THEME_GIT_PROMPT_CLEAN=" \u2714"

if [ -e ~/.rvm/bin/rvm-prompt ]; then
  RPROMPT='%{$FG[196]%}‹$(rvm_current)›%{$reset_color%}'
else
  if which rbenv &> /dev/null; then
    RPROMPT='%{$FG[196]%}$(rbenv_version)%{$reset_color%}'
  fi
fi

