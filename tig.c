#include <sys/types.h>
#include <regex.h>

#define SIZEOF_STR	1024	/* Default string size. */
#define SIZEOF_REV	41	/* Holds a SHA-1 and an ending NUL */
#define TIG_TREE_CMD	\
	"git ls-tree %s %s"

#define TIG_BLOB_CMD	\
	"git cat-file blob %s"

	char id[SIZEOF_REV];	/* Commit SHA1 ID */
string_ncopy_do(char *dst, size_t dstlen, const char *src, size_t srclen)
	if (srclen > dstlen - 1)
		srclen = dstlen - 1;
	strncpy(dst, src, srclen);
	dst[srclen] = 0;
/* Shorthands for safely copying into a fixed buffer. */

	string_ncopy_do(dst, sizeof(dst), src, sizeof(dst))

#define string_ncopy(dst, src, srclen) \
	string_ncopy_do(dst, sizeof(dst), src, srclen)
string_nformat(char *buf, size_t bufsize, size_t *bufpos, const char *fmt, ...)
	size_t pos = bufpos ? *bufpos : 0;
sq_quote(char buf[SIZEOF_STR], size_t bufsize, const char *src)
#define BUFPUT(x) do { if (bufsize < SIZEOF_STR) buf[bufsize++] = (x); } while (0)
	REQ_(VIEW_TREE,		"Show tree view"), \
	REQ_(VIEW_BLOB,		"Show blob view"), \
	REQ_GROUP("Searching") \
	REQ_(SEARCH,		"Search the view"), \
	REQ_(SEARCH_BACK,	"Search backwards in the view"), \
	REQ_(FIND_NEXT,		"Find next search match"), \
	REQ_(FIND_PREV,		"Find previous search match"), \
	\
	REQ_(NONE,		"Do nothing"), \
static bool opt_line_number		= FALSE;
static bool opt_rev_graph		= TRUE;
static int opt_num_interval		= NUMBER_INTERVAL;
static int opt_tab_size			= TABSIZE;
static enum request opt_request		= REQ_VIEW_MAIN;
static char opt_cmd[SIZEOF_STR]		= "";
static char opt_path[SIZEOF_STR]	= "";
static FILE *opt_pipe			= NULL;
static char opt_encoding[20]		= "UTF-8";
static bool opt_utf8			= TRUE;
static char opt_codeset[20]		= "UTF-8";
static iconv_t opt_iconv		= ICONV_NONE;
static char opt_search[SIZEOF_STR]	= "";
LINE(ACKED,	   "    Acked-by",	COLOR_YELLOW,	COLOR_DEFAULT,	0), \
LINE(TREE_DIR,     "",			COLOR_DEFAULT,	COLOR_DEFAULT,	A_NORMAL), \
LINE(TREE_FILE,    "",			COLOR_DEFAULT,	COLOR_DEFAULT,	A_NORMAL)

	/* State flags */
	unsigned int selected:1;

	{ 't',		REQ_VIEW_TREE },
	{ 'f',		REQ_VIEW_BLOB },
	/* Searching */
	{ '/',		REQ_SEARCH },
	{ '?',		REQ_SEARCH_BACK },
	{ 'n',		REQ_FIND_NEXT },
	{ 'N',		REQ_FIND_PREV },

	{ '.',		REQ_TOGGLE_LINENO },
	{ ERR,		REQ_NONE },
	/* Using the ncurses SIGWINCH handler. */
	KEYMAP_(TREE), \
	KEYMAP_(BLOB), \
	size_t pos = 0;
	char buf[SIZEOF_STR];
#define foreach_displayed_view(view, i) \
static char ref_blob[SIZEOF_REF]	= "";
	const char *id;		/* Points to either of ref_{head,commit,blob} */
	char cmd[SIZEOF_STR];	/* Command buffer */
	/* Searching */
	char grep[SIZEOF_STR];	/* Search string */
	regex_t *regex;		/* Pre-compiled regex */

	bool (*draw)(struct view *view, struct line *line, unsigned int lineno, bool selected);
	/* Search for regex in a line. */
	bool (*grep)(struct view *view, struct line *line);
	/* Select line */
	void (*select)(struct view *view, struct line *line);
static struct view_ops tree_ops;
static struct view_ops blob_ops;
	VIEW_(TREE,  "tree",  &tree_ops,  ref_commit),
	VIEW_(BLOB,  "blob",  &blob_ops,  ref_blob),
#define foreach_view(view, i) \
	for (i = 0; i < ARRAY_SIZE(views) && (view = &views[i]); i++)

#define view_is_displayed(view) \
	(view == display[0] || view == display[1])
	struct line *line;
	bool selected = (view->offset + lineno == view->lineno);
	bool draw_ok;

	assert(view_is_displayed(view));

	line = &view->line[view->offset + lineno];

	if (selected) {
		line->selected = TRUE;
		view->ops->select(view, line);
	} else if (line->selected) {
		line->selected = FALSE;
		wmove(view->win, lineno, 0);
		wclrtoeol(view->win);
	}

	scrollok(view->win, FALSE);
	draw_ok = view->ops->draw(view, line, lineno, selected);
	scrollok(view->win, TRUE);

	return draw_ok;
	char buf[SIZEOF_STR];
	char state[SIZEOF_STR];
	size_t bufpos = 0, statelen = 0;
	assert(view_is_displayed(view));
		string_format_from(state, &statelen, "- %s %d of %d (%d%%)",
				   view->ops->type,
				   view->lineno + 1,
				   view->lines,
				   lines);

		if (view->pipe) {
			time_t secs = time(NULL) - view->start_time;

			/* Three git seconds are a long time ... */
			if (secs > 2)
				string_format_from(state, &statelen, " %lds", secs);
		}
	string_format_from(buf, &bufpos, "[%s]", view->name);
	if (*view->ref && bufpos < view->width) {
		size_t refsize = strlen(view->ref);
		size_t minsize = bufpos + 1 + /* abbrev= */ 7 + 1 + statelen;
		if (minsize < view->width)
			refsize = view->width - minsize + 7;
		string_format_from(buf, &bufpos, " %.*s", refsize, view->ref);
	if (statelen && bufpos < view->width) {
		string_format_from(buf, &bufpos, " %s", state);
	}

	if (view == display[current_view])
		wbkgdset(view->title, get_line_attr(LINE_TITLE_FOCUS));
	else
		wbkgdset(view->title, get_line_attr(LINE_TITLE_BLUR));

	werase(view->title);
	mvwaddnstr(view->title, 0, 0, buf, bufpos);
	foreach_displayed_view (view, i) {
	foreach_displayed_view (view, i) {
update_display_cursor(struct view *view)
do_scroll_view(struct view *view, int lines)
	bool redraw_current_line = FALSE;

	/* Move current line into the view. */
	if (view->lineno < view->offset) {
		view->lineno = view->offset;
		redraw_current_line = TRUE;
	} else if (view->lineno >= view->offset + view->height) {
		view->lineno = view->offset + view->height - 1;
		redraw_current_line = TRUE;
	}

	assert(view->offset <= view->lineno && view->lineno < view->lines);

		if (redraw_current_line)
			draw_view_line(view, view->lineno - view->offset);
	assert(view_is_displayed(view));

	do_scroll_view(view, lines);
move_view(struct view *view, enum request request)
	int scroll_steps = 0;
		scroll_steps = steps;
			scroll_steps = -view->offset;
				scroll_steps = view->lines - view->offset - 1;
				if (scroll_steps >= view->height)
					scroll_steps -= view->height - 1;
	}
	if (!view_is_displayed(view)) {
		view->offset += steps;
		view->ops->select(view, &view->line[view->lineno]);
	/* Repaint the old "current" line if we be scrolling */
	if (ABS(steps) < view->height)
		draw_view_line(view, view->lineno - steps - view->offset);
	if (scroll_steps) {
		do_scroll_view(view, scroll_steps);
	}

	/* Draw the current line */
	draw_view_line(view, view->lineno - view->offset);
/*
 * Searching
 */

static void search_view(struct view *view, enum request request);

static bool
find_next_line(struct view *view, unsigned long lineno, struct line *line)
{
	assert(view_is_displayed(view));

	if (!view->ops->grep(view, line))
		return FALSE;

	if (lineno - view->offset >= view->height) {
		view->offset = lineno;
		view->lineno = lineno;
		redraw_view(view);

	} else {
		unsigned long old_lineno = view->lineno - view->offset;

		view->lineno = lineno;
		draw_view_line(view, old_lineno);

		draw_view_line(view, view->lineno - view->offset);
		redrawwin(view->win);
		wrefresh(view->win);
	}

	report("Line %ld matches '%s'", lineno + 1, view->grep);
	return TRUE;
}

static void
find_next(struct view *view, enum request request)
{
	unsigned long lineno = view->lineno;
	int direction;

	if (!*view->grep) {
		if (!*opt_search)
			report("No previous search");
		else
			search_view(view, request);
		return;
	}

	switch (request) {
	case REQ_SEARCH:
	case REQ_FIND_NEXT:
		direction = 1;
		break;

	case REQ_SEARCH_BACK:
	case REQ_FIND_PREV:
		direction = -1;
		break;

	default:
		return;
	}

	if (request == REQ_FIND_NEXT || request == REQ_FIND_PREV)
		lineno += direction;

	/* Note, lineno is unsigned long so will wrap around in which case it
	 * will become bigger than view->lines. */
	for (; lineno < view->lines; lineno += direction) {
		struct line *line = &view->line[lineno];

		if (find_next_line(view, lineno, line))
			return;
	}

	report("No match found for '%s'", view->grep);
}

static void
search_view(struct view *view, enum request request)
{
	int regex_err;

	if (view->regex) {
		regfree(view->regex);
		*view->grep = 0;
	} else {
		view->regex = calloc(1, sizeof(*view->regex));
		if (!view->regex)
			return;
	}

	regex_err = regcomp(view->regex, opt_search, REG_EXTENDED);
	if (regex_err != 0) {
		char buf[SIZEOF_STR] = "unknown error";

		regerror(regex_err, view->regex, buf, sizeof(buf));
		report("Search failed: %s", buf);
		return;
	}

	string_copy(view->grep, opt_search);

	find_next(view, request);
}


	} else if (view == VIEW(REQ_VIEW_TREE)) {
		const char *format = view->cmd_env ? view->cmd_env : view->cmd_fmt;

		if (strcmp(view->vid, view->id))
			opt_path[0] = 0;

		if (!string_format(view->cmd, format, id, opt_path))
			return FALSE;

	/* FIXME: This is probably not perfect for backgrounded views. */
	if (!view_is_displayed(view))
		goto check_pipe;

	if (view == VIEW(REQ_VIEW_TREE)) {
		/* Clear the view and redraw everything since the tree sorting
		 * might have rearranged things. */
		redraw_view(view);

	} else if (redraw_from >= 0) {
check_pipe:
		do_scroll_view(prev, lines);
		move_view(view, request);
	case REQ_VIEW_BLOB:
		if (!ref_blob[0]) {
			report("No file chosen, press 't' to open tree view");
			break;
		}
		/* Fall-through */
	case REQ_VIEW_TREE:
		if ((view == VIEW(REQ_VIEW_DIFF) &&
		     view->parent == VIEW(REQ_VIEW_MAIN)) ||
		   (view == VIEW(REQ_VIEW_BLOB) &&
		     view->parent == VIEW(REQ_VIEW_TREE))) {
			move_view(view, request);
			if (view_is_displayed(view))
			move_view(view, request);
	case REQ_SEARCH:
	case REQ_SEARCH_BACK:
		search_view(view, request);
		break;

	case REQ_FIND_NEXT:
	case REQ_FIND_PREV:
		find_next(view, request);
		break;

	case REQ_NONE:
pager_draw(struct view *view, struct line *line, unsigned int lineno, bool selected)
	if (selected) {
static bool
add_describe_ref(char *buf, size_t *bufpos, char *commit_id, const char *sep)
{
	char refbuf[SIZEOF_STR];
	char *ref = NULL;
	FILE *pipe;

	if (!string_format(refbuf, "git describe %s", commit_id))
		return TRUE;

	pipe = popen(refbuf, "r");
	if (!pipe)
		return TRUE;

	if ((ref = fgets(refbuf, sizeof(refbuf), pipe)))
		ref = chomp_string(ref);
	pclose(pipe);

	if (!ref || !*ref)
		return TRUE;

	/* This is the only fatal call, since it can "corrupt" the buffer. */
	if (!string_nformat(buf, SIZEOF_STR, bufpos, "%s%s", sep, ref))
		return FALSE;

	return TRUE;
}

	char buf[SIZEOF_STR];
	char *commit_id = line->data + STRING_SIZE("commit ");
	size_t bufpos = 0, refpos = 0;
	bool is_tag = FALSE;
	refs = get_refs(commit_id);
	if (!refs) {
		if (view == VIEW(REQ_VIEW_DIFF))
			goto try_add_describe_ref;
	}
		if (ref->tag)
			is_tag = TRUE;
	if (!is_tag && view == VIEW(REQ_VIEW_DIFF)) {
try_add_describe_ref:
		/* Add <tag>-g<commit_id> "fake" reference. */
		if (!add_describe_ref(buf, &bufpos, commit_id, sep))
			return;
	}

	if (bufpos == 0)
		return;

static bool
pager_grep(struct view *view, struct line *line)
{
	regmatch_t pmatch;
	char *text = line->data;

	if (!*text)
		return FALSE;

	if (regexec(view->regex, text, 1, &pmatch, 0) == REG_NOMATCH)
		return FALSE;

	return TRUE;
}

static void
pager_select(struct view *view, struct line *line)
{
	if (line->type == LINE_COMMIT) {
		char *text = line->data;

		string_copy(view->ref, text + STRING_SIZE("commit "));
		string_copy(ref_commit, view->ref);
	}
}

	pager_grep,
	pager_select,
};


/*
 * Tree backend
 */

/* Parse output from git-ls-tree(1):
 *
 * 100644 blob fb0e31ea6cc679b7379631188190e975f5789c26	Makefile
 * 100644 blob 5304ca4260aaddaee6498f9630e7d471b8591ea6	README
 * 100644 blob f931e1d229c3e185caad4449bf5b66ed72462657	tig.c
 * 100644 blob ed09fe897f3c7c9af90bcf80cae92558ea88ae38	web.conf
 */

#define SIZEOF_TREE_ATTR \
	STRING_SIZE("100644 blob ed09fe897f3c7c9af90bcf80cae92558ea88ae38\t")

#define TREE_UP_FORMAT "040000 tree %s\t.."

static int
tree_compare_entry(enum line_type type1, char *name1,
		   enum line_type type2, char *name2)
{
	if (type1 != type2) {
		if (type1 == LINE_TREE_DIR)
			return -1;
		return 1;
	}

	return strcmp(name1, name2);
}

static bool
tree_read(struct view *view, char *text)
{
	size_t textlen = strlen(text);
	char buf[SIZEOF_STR];
	unsigned long pos;
	enum line_type type;
	bool first_read = view->lines == 0;

	if (textlen <= SIZEOF_TREE_ATTR)
		return FALSE;

	type = text[STRING_SIZE("100644 ")] == 't'
	     ? LINE_TREE_DIR : LINE_TREE_FILE;

	if (first_read) {
		/* Add path info line */
		if (string_format(buf, "Directory path /%s", opt_path) &&
		    realloc_lines(view, view->line_size + 1) &&
		    pager_read(view, buf))
			view->line[view->lines - 1].type = LINE_DEFAULT;
		else
			return FALSE;

		/* Insert "link" to parent directory. */
		if (*opt_path &&
		    string_format(buf, TREE_UP_FORMAT, view->ref) &&
		    realloc_lines(view, view->line_size + 1) &&
		    pager_read(view, buf))
			view->line[view->lines - 1].type = LINE_TREE_DIR;
		else if (*opt_path)
			return FALSE;
	}

	/* Strip the path part ... */
	if (*opt_path) {
		size_t pathlen = textlen - SIZEOF_TREE_ATTR;
		size_t striplen = strlen(opt_path);
		char *path = text + SIZEOF_TREE_ATTR;

		if (pathlen > striplen)
			memmove(path, path + striplen,
				pathlen - striplen + 1);
	}

	/* Skip "Directory ..." and ".." line. */
	for (pos = 1 + !!*opt_path; pos < view->lines; pos++) {
		struct line *line = &view->line[pos];
		char *path1 = ((char *) line->data) + SIZEOF_TREE_ATTR;
		char *path2 = text + SIZEOF_TREE_ATTR;
		int cmp = tree_compare_entry(line->type, path1, type, path2);

		if (cmp <= 0)
			continue;

		text = strdup(text);
		if (!text)
			return FALSE;

		if (view->lines > pos)
			memmove(&view->line[pos + 1], &view->line[pos],
				(view->lines - pos) * sizeof(*line));

		line = &view->line[pos];
		line->data = text;
		line->type = type;
		view->lines++;
		return TRUE;
	}

	if (!pager_read(view, text))
		return FALSE;

	/* Move the current line to the first tree entry. */
	if (first_read)
		view->lineno++;

	view->line[view->lines - 1].type = type;
	return TRUE;
}

static bool
tree_enter(struct view *view, struct line *line)
{
	enum open_flags flags = display[0] == view ? OPEN_SPLIT : OPEN_DEFAULT;
	enum request request;

	switch (line->type) {
	case LINE_TREE_DIR:
		/* Depending on whether it is a subdir or parent (updir?) link
		 * mangle the path buffer. */
		if (line == &view->line[1] && *opt_path) {
			size_t path_len = strlen(opt_path);
			char *dirsep = opt_path + path_len - 1;

			while (dirsep > opt_path && dirsep[-1] != '/')
				dirsep--;

			dirsep[0] = 0;

		} else {
			size_t pathlen = strlen(opt_path);
			size_t origlen = pathlen;
			char *data = line->data;
			char *basename = data + SIZEOF_TREE_ATTR;

			if (!string_format_from(opt_path, &pathlen, "%s/", basename)) {
				opt_path[origlen] = 0;
				return TRUE;
			}
		}

		/* Trees and subtrees share the same ID, so they are not not
		 * unique like blobs. */
		flags |= OPEN_RELOAD;
		request = REQ_VIEW_TREE;
		break;

	case LINE_TREE_FILE:
		request = REQ_VIEW_BLOB;
		break;

	default:
		return TRUE;
	}

	open_view(view, request, flags);

	return TRUE;
}

static void
tree_select(struct view *view, struct line *line)
{
	char *text = line->data;

	text += STRING_SIZE("100644 blob ");

	if (line->type == LINE_TREE_FILE) {
		string_ncopy(ref_blob, text, 40);
		/* Also update the blob view's ref, since all there must always
		 * be in sync. */
		string_copy(VIEW(REQ_VIEW_BLOB)->ref, ref_blob);

	} else if (line->type != LINE_TREE_DIR) {
		return;
	}

	string_ncopy(view->ref, text, 40);
}

static struct view_ops tree_ops = {
	"file",
	pager_draw,
	tree_read,
	tree_enter,
	pager_grep,
	tree_select,
};

static bool
blob_read(struct view *view, char *line)
{
	bool state = pager_read(view, line);

	if (state == TRUE)
		view->line[view->lines - 1].type = LINE_DEFAULT;

	return state;
}

static struct view_ops blob_ops = {
	"line",
	pager_draw,
	blob_read,
	pager_enter,
	pager_grep,
	pager_select,
	char id[SIZEOF_REV];		/* SHA1 ID. */
main_draw(struct view *view, struct line *line, unsigned int lineno, bool selected)
	if (selected) {
static bool
main_grep(struct view *view, struct line *line)
{
	struct commit *commit = line->data;
	enum { S_TITLE, S_AUTHOR, S_DATE, S_END } state;
	char buf[DATE_COLS + 1];
	regmatch_t pmatch;

	for (state = S_TITLE; state < S_END; state++) {
		char *text;

		switch (state) {
		case S_TITLE:	text = commit->title;	break;
		case S_AUTHOR:	text = commit->author;	break;
		case S_DATE:
			if (!strftime(buf, sizeof(buf), DATE_FORMAT, &commit->time))
				continue;
			text = buf;
			break;

		default:
			return FALSE;
		}

		if (regexec(view->regex, text, 1, &pmatch, 0) != REG_NOMATCH)
			return TRUE;
	}

	return FALSE;
}

static void
main_select(struct view *view, struct line *line)
{
	struct commit *commit = line->data;

	string_copy(view->ref, commit->id);
	string_copy(ref_commit, view->ref);
}

	main_grep,
	main_select,
	update_display_cursor(view);
static char *
read_prompt(const char *prompt)
	static char buf[sizeof(opt_cmd) - STRING_SIZE("git \0")];
		report("%s%.*s", prompt, pos, buf);
				return NULL;
		return NULL;
	return buf;
		if (opt_iconv == ICONV_NONE)
		{
			char *cmd = read_prompt(":");

			if (cmd && string_format(opt_cmd, "git %s", cmd)) {
				if (strncmp(cmd, "show", 4) && isspace(cmd[4])) {
					opt_request = REQ_VIEW_DIFF;
				} else {
					opt_request = REQ_VIEW_PAGER;
				}
				break;
			}

			request = REQ_NONE;
		}
		case REQ_SEARCH:
		case REQ_SEARCH_BACK:
		{
			const char *prompt = request == REQ_SEARCH
					   ? "/" : "?";
			char *search = read_prompt(prompt);
			if (search)
				string_copy(opt_search, search);
			else
				request = REQ_NONE;
			break;
		}