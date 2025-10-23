
#include "../../../includes/minishell.h"

  char *heredoc_value_expansion(char *word, bool in_quote, int fd, size_t total_len)
  {
    char *buf;
    if (fd==0)
    {
      if (word==NULL)
        return (NULL);
      buf = strdup(word);
    }
    else
    {
      buf = malloc(sizeof(char)*(total_len+1));
      if (buf==NULL)
      {
        perror("malloc: ");
        return (NULL);
      }
    }
    while (1)
    {
      ssize_t rb = 0;
      rb = read(fd, buf, total_len);
      if (rb<0)
      {
        perror("read: ");
        return (NULL);
      }
      else if (rb==0)
        break ;
    }
    //if $ comes, then consume the idx and concatenate word.
  }

char *heredoc_expansion(char *word, bool in_quote, int fd, size_t total_len)
{
  size_t len=0;

    size_t len = 0;
      if (word==NULL || word[0]=='\0')
      {
        len = 0;
        return (word);
      }
      if (in_quote)
      {
        len=strlen(word);
        return (word);
      }

  char *value = heredoc_value_expansion(word, in_quote, &len);
  //if quoted, non expansion $.

}

int get_tmp_fd(char *src, size_t size)
{
  int tmp_fd = ft_mkstmpfd(TMP_TEMPLATE);
  if (tmp_fd<0)
    return(-1);
  write(tmp_fd, src, size);
  return tmp_fd;
}

int heredoc_loop(char **value, t_redir *hd)
{
  char *line = NULL;
  size_t line_len = 0;
  line = get_next_line();
  size_t delim_len = strlen(hd->filename);
  size_t total_len = 0;
  char *buf = NULL;
  if (value==NULL)
    return (-1);

  if (total_len>PIPE_SIZE)
      goto USE_TMP;
  while (strncmp(line, hd->filename, delim_len)!=0)
  {
    // write(tmp_fd, line, line_len);
    if (total_len>PIPE_SIZE)
      goto USE_TMP;
    *value = realloc(buf, sizeof(char)*(total_len + 1));
    if (!*value)
    {
      perror("realloc: ");
      return (-1);
    }
    bzero(*value, sizeof(char)*(total_len+1));
    strncpy(value + total_len - line_len - 1, line, line_len+1);
    free(line);
    line = get_next_line();
    if (line==NULL)
      return (0);
    line_len = strlen(line);
    total_len += line_len;
  }
  return (1);
  USE_TMP:
    int tmp_fd = get_tmp_fd(*value, total_len);
  if (total_len>PIPE_SIZE)
  {
    while (strncmp(line, hd->filename, delim_len)!=0)
    {
      write(tmp_fd, line, line_len);
      free(line);
      line = get_next_line(tmp_fd);
      if (line==NULL)
        return (0);
      line_len = strlen(line);
      total_len += line_len;
    }
  }
  free(line);
  free(*value);
  return (0);
}

// static int
// heredoc_write (int fd, const char *heredoc, size_t herelen)
// {
//   ssize_t nw;
//   int e;

//   errno = 0;
//   nw = write (fd, heredoc, herelen);
//   e = errno;
//   if (nw != herelen)
//     {
//       if (e == 0)
// 	e = ENOSPC;
//       return e;
//     }
//   return 0;
// }

/* Create a temporary file or pipe holding the text of the here document
   pointed to by REDIRECTEE, and return a file descriptor open for reading
   to it. Return -1 on any error, and make sure errno is set appropriately. */


// static int
// here_document_to_fd (WORD_DESC *redirectee, enum r_instruction ri)
// {
//   char *filename;
//   int r, fd, fd2, herepipe[2];
//   char *document;
//   size_t document_len;
// #if HEREDOC_PARANOID
//   struct stat st1, st2;
// #endif

//   /* Expand the here-document/here-string first and then decide what to do. */
//   document = heredoc_expand (redirectee, ri, &document_len);

//   /* If we have a zero-length document, don't mess with a temp file */
//   if (document_len == 0)
//     {
//       fd = open ("/dev/null", O_RDONLY);
//       r = errno;
//       if (document != redirectee->word)
// 	FREE (document);
//       errno = r;
//       return fd;
//     }

//   if (shell_compatibility_level <= 50)
//     goto use_tempfile;

// #if HEREDOC_PIPESIZE
//   /* Try to use a pipe internal to this process if the document is shorter
//      than the system's pipe capacity (computed at build time). We want to
//      write the entire document without write blocking. */
//   if (document_len <= HEREDOC_PIPESIZE)
//     {
//       if (pipe (herepipe) < 0)
// 	{
// 	  /* XXX - goto use_tempfile; ? */
// 	  r = errno;
// 	  if (document != redirectee->word)
// 	    free (document);
// 	  errno = r;
// 	  return (-1);
// 	}

// #if defined (F_GETPIPE_SZ)
//       if (fcntl (herepipe[1], F_GETPIPE_SZ, 0) < document_len)
// 	{
// 	  close (herepipe[0]);
// 	  close (herepipe[1]);
// 	  goto use_tempfile;
// 	}
// #endif

//       r = heredoc_write (herepipe[1], document, document_len);
//       if (document != redirectee->word)
// 	free (document);
//       close (herepipe[1]);
//       if (r)			/* write error */
// 	{
// 	  close (herepipe[0]);
// 	  errno = r;
// 	  return (-1);
// 	}
//       return (herepipe[0]);
//     }
// #endif

// use_tempfile:

//   /* TAG: use anonfiles here in a future version. */
//   fd = sh_mktmpfd ("sh-thd", MT_USERANDOM|MT_USETMPDIR, &filename);

//   /* If we failed for some reason other than the file existing, abort */
//   if (fd < 0)
//     {
//       r = errno;
//       FREE (filename);
//       if (document != redirectee->word)
// 	FREE (document);
//       errno = r;
//       return (fd);
//     }

//   fchmod (fd, S_IRUSR | S_IWUSR);
//   SET_CLOSE_ON_EXEC (fd);

//   errno = r = 0;		/* XXX */
//   r = heredoc_write (fd, document, document_len);
//   if (document != redirectee->word)
//     FREE (document);

//   if (r)
//     {
//       close (fd);
//       unlink (filename);
//       free (filename);
//       errno = r;
//       return (-1);
//     }

//   /* In an attempt to avoid races, we close the first fd only after opening
//      the second. */
//   /* Make the document really temporary.  Also make it the input. */
//   fd2 = open (filename, O_RDONLY|O_BINARY, 0600);

//   if (fd2 < 0)
//     {
//       r = errno;
//       unlink (filename);
//       free (filename);
//       close (fd);
//       errno = r;
//       return -1;
//     }

// #if HEREDOC_PARANOID
//   /* We can use same_file here to check whether or not fd and fd2 refer to
//      the same file, but we don't do that unless HEREDOC_PARANOID is defined. */
//   if (fstat (fd, &st1) < 0 || S_ISREG (st1.st_mode) == 0 ||
//       fstat (fd2, &st2) < 0 || S_ISREG (st2.st_mode) == 0 ||
//       same_file (filename, filename, &st1, &st2) == 0)
//     {
//       unlink (filename);
//       free (filename);
//       close (fd);
//       close (fd2);
//       errno = EEXIST;
//       return -1;
//     }
// #endif

//   close (fd);
//   if (unlink (filename) < 0)
//     {
//       r = errno;
//       close (fd2);
//       free (filename);
//       errno = r;
//       return (-1);
//     }

//   free (filename);

//   fchmod (fd2, S_IRUSR);
//   return (fd2);
// }