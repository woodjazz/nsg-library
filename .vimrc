"load this file with command:
":so .vimrc

set nocompatible
filetype plugin on

set tabstop=4 shiftwidth=4 expandtab

set hlsearch
set title

"set always visible status line
"set laststatus=2

"Add full file path to your existing statusline
"set statusline+=%F

"Press Space to turn off highlighting and clear any message already displayed.
nnoremap <silent> <Space> :nohlsearch<Bar>:echo<CR>

"to use gf (go to file)
set path=**
set number

"ctags
"sudo apt-get install exuberant-ctags
:nnoremap <f5> :!ctags -R<CR>

"buffers
nnoremap <silent> <C-S-Left> :bprevious<CR>
nnoremap <silent> <C-S-Right> :bnext<CR>
nnoremap <silent> <C-S-Up> :bfirst<CR>
nnoremap <silent> <C-S-Down> :blast<CR>

"enable system clipboard (only works with gvim)
"set clipboard=unnamed
