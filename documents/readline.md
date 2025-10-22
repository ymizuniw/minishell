Function: int rl_on_new_line (void)
Tell the update functions that we have moved onto a new (empty) line, usually after outputting a newline.

rl_clear_history (void)
Clear the history list by deleting all of the entries, in the same manner as the History library’s clear_history() function. This differs from clear_history because it frees private data Readline saves in the history list.

Function: void rl_replace_line (const char *text, int clear_undo)
Replace the contents of rl_line_buffer with text. This preserves the point and mark, if possible. If clear_undo is non-zero, this clears the undo list associated with the current line.

Function: void rl_redisplay (void)
Change what’s displayed on the screen to reflect the current contents of rl_line_buffer.

accept-line (Newline or Return)
Accept the line regardless of where the cursor is. If this line is non-empty, you can add it to the history list using add_history(). If this line is a modified history line, then restore the history line to its original state.

USAGE:

readline(const char *prompt)	
ユーザー入力を取得する	
メインループの各サイクルの先頭
標準入力から1行読み込む。
入力終了時は NULL を返す（ctrl-D 判定に使う）。if (line==NULL)

add_history(const char *line)	
入力コマンドを履歴に登録する	
コマンドを正常に読み込んだ直後	
空行やヒアドキュメント入力などは履歴に追加しない。if (s[0]=='\0'), here_doc, 

rl_on_new_line(void)	
シグナル割り込み時などに「新しい行」を表示する準備	SIGINT ハンドラ内	readline内部バッファを「次の入力行」に移行する。

rl_replace_line(const char *text, int clear_undo)	
現在の入力行を置き換える（削除）	SIGINT ハンドラ内	例：ctrl-C を押したとき入力中の内容を消去する。

rl_redisplay(void)	
現在のプロンプトと入力行を再描画する	SIGINT ハンドラ内	rl_replace_line＋rl_on_new_lineの後に呼ぶ。

rl_clear_history(void)	プログラム終了時に履歴を解放する	minishell終了時	add_history()で確保された履歴を全削除。
