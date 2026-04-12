#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
  const char* data;
  size_t count;
} sview;

sview sv(const char* Cstring)
{
  return (sview){ 
    .data = Cstring,
    .count = strlen(Cstring),
  };
}

void sview_chop_right(sview* sv, size_t n)
{
  if (sv->count == 0) return;
  sv->count -= n;
}

void sview_chop_left(sview* sv, size_t n)
{
  if (sv->count == 0) return;
  sv->count -= n;
  sv->data  += n;
}

void sview_trim_left(sview* sv)
{
  while (sv->count > 0 && isspace(sv->data[0])) {
    sview_chop_left(sv, 1);
  }
}

void sview_trim_right(sview* sv)
{
  while (sv->count > 0 && isspace(sv->data[sv->count-1])) {
    sview_chop_right(sv, 1); 
  }
}

void sview_trim(sview* sv)
{
  sview_trim_left(sv);
  sview_trim_right(sv);
}

sview slice(sview* sv, char separator)
{
  size_t i = 0;
  while (i < sv->count && sv->data[i] != separator) {
    ++i; 
  }
  if (i < sv->count) {
      sview result = {
        .data = sv->data,
        .count = i,
    };
    sview_chop_left(sv, i + 1);
    return result;
  }
  sview result = *sv;
  sview_chop_left(sv, sv->count);
  return result;
}
//TODO: Back converter - converts string view back to a Null terminated string for backward compatability
//      String view funtions:
//        capitalize()
//        casefold()
//        center(width, fillchar)
//        count(x, start, end)
//        encode(encoding, errors)
//        endswith(suffix, start, end)
//        expandtabs(tabsize)
//        find(sub, start, end)

sview capitalize(sview sv)
{
  char buffer[512];
  strncpy(buffer, sv.data, sv.count);
  size_t i = 0;
  while (i < sv.count) {
    buffer[i] = toupper(sv.data[i]);
    ++i; 
  }
  sview result = {
    .data = buffer,
    .count = strlen(buffer),
  };
  return result;
}

#define STRING_VIEW_FORMATTING "%.*s"
#define STRING_VIEW_ARGS(s) (s).count, (s).data

int main(int argc, char* argv[])
{
  sview name = sv("Nick");
  printf("%s, its yours first clean C project, do you like it?\n", name.data);
  char buffer[64];
  fgets(buffer, sizeof(buffer),stdin);
  puts(buffer);
  sview test_slice = sv("This, and that");
  sview this = slice(&test_slice, ',');
  printf("|"STRING_VIEW_FORMATTING"|\n", STRING_VIEW_ARGS(this));
  sview upper = capitalize(name);
  printf("|"STRING_VIEW_FORMATTING"|\n", STRING_VIEW_ARGS(upper));
  return 0;
}
