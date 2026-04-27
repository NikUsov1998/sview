#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
  const char* data;
  size_t count;
} sview;

typedef struct
{
  sview** items;
  size_t count;
} sview_array_t;

#define sv_array(...) (sview_array_t){ \
  .items = (sview*[]){__VA_ARGS__}, \
  .count = sizeof((sview*[]){__VA_ARGS__}) / sizeof(sview*) \
}

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

sview Upper(sview sv)
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

sview Lower(sview sv)
{
  char buffer[512];
  strncpy(buffer, sv.data, sv.count);
  size_t i = 0;
  while (i < sv.count) {
    buffer[i] = tolower(sv.data[i]);
    ++i; 
  }
  sview result = {
    .data = buffer,
    .count = strlen(buffer),
  };
  return result;
}

void printsv(sview sv)
{
  #define STRING_VIEW_FORMATTING "%.*s"
  #define STRING_VIEW_ARGS(s) (s).count, (s).data
  printf(""STRING_VIEW_FORMATTING"\n", STRING_VIEW_ARGS(sv));
}

sview title(sview str_view)
{
  char buffer[1024] = {};

  if (str_view.count > sizeof(buffer)) {
    sview error = sv("String are too long!"); 
    return error;
  }

  for (int i = 0; i < str_view.count; ++i) {
    if ((islower(str_view.data[i]) && isspace(str_view.data[i-1])) || 
        (islower(str_view.data[i]) && i == 0)) {
      buffer[i] = toupper(str_view.data[i]);
    }
    else {
      buffer[i] = str_view.data[i];
    }
  }

  sview result = {
    .data = buffer,
    .count = strlen(buffer),
  };
  return result;

}

sview capitalize(sview str_view)
{
  char buffer[1024] = {};

  for (int i = 0; i < str_view.count; ++i) {
    if (islower(str_view.data[i]) && i == 0) {
      buffer[i] = toupper(str_view.data[i]);
    }
    else {
      buffer[i] = str_view.data[i];
    }
  }

  sview result = {
    .data = buffer,
    .count = strlen(buffer),
  };
  return result;
}

sview center(sview sv, int width, char filler)
{
  char buffer[width];
  memset(buffer, filler, sizeof(buffer));
  buffer[width] = '\0';
  int base_center = sizeof(buffer) / 2;
  int sv_center = sv.count / 2;
  int true_center = base_center - sv_center;
  for (int i = 0; i < sv.count; ++i) {
    buffer[true_center + i] = sv.data[i];
  }
  sview result = {
    .data = buffer,
    .count = strlen(buffer),
  };
  return result;
}

sview left(sview sv, int width, char filler)
{
  char buffer[width];
  memset(buffer, filler, sizeof(buffer));
  buffer[width] = '\0';
  for (int i = 0; i < sv.count; ++i) {
    buffer[i] = sv.data[i];  
  }
  sview result = {
    .data = buffer,
    .count = strlen(buffer),
  };
  return result;
}

sview right(sview sv, int width, char filler)
{
  char buffer[width];
  memset(buffer, filler, sizeof(buffer));
  buffer[width] = '\0';
  int offset = width - sv.count;
  for (int i = 0; i < sv.count; ++i) {
    buffer[offset + i] = sv.data[i]; 
  }
  sview result = {
    .data = buffer,
    .count = sizeof(buffer),
  };
  return result;
}
sview join(const sview_array_t* array, char* separator)
{
  size_t buffer_size = 0;
  for (size_t i = 0; i < array->count; ++i) {
    buffer_size += array->items[i]->count;

    if (i < array->count - 1) {
      buffer_size += strlen(separator);
    }
  }
  
  char* buffer = malloc(buffer_size);

  if (!buffer) {
    return (sview){NULL, 0};
  }

  int ptr = 0;
  for (size_t i = 0; i < array->count; ++i) {
    
    int tmp = 0;
    for (int j = 0; j < array->items[i]->count; ++j) {
      tmp = ptr + j;
      buffer[tmp] = array->items[i]->data[j];
    }

    ptr = tmp + 1;
    if (i < array->count - 1) {
      buffer[ptr] = *separator;
      ++ptr;
    }
  }

  sview result = {
    .data = buffer,
    .count = strlen(buffer),
  };

  return result;
}

char* converter(sview sv, char** result)
{
  //char result[sv.count + 1];
  //memcpy(*result, sv.data, sv.count + 1);
  for (int i = 0; i < sv.count; ++i) {
    *result[i] = sv.data[i];
  }
  result[sv.count + 1] = '\0';

  return *result;
}

bool endswith(sview string, sview suffix, int start, int end)
{
  sview tmp = string;
  int offset = tmp.count - end;
  sview_chop_right(&tmp, offset);
  sview_chop_left(&tmp, start);

  char buffer[tmp.count];
  buffer[tmp.count] = '\0';

  for (int i = 0; i < sizeof(buffer); ++i) {
    buffer[i] = tmp.data[i]; 
  }

  sview sv_res = {
    .data = buffer,
    .count = strlen(buffer),
  };

  int result = strcmp(&sv_res.data[sv_res.count - suffix.count], suffix.data);
  
  if (result == 0) {
    return true;
  }
  return false;
}

bool startswith(sview string, sview suffix, int start, int end)
{
  sview tmp = string;
  int offset = tmp.count - end;
  sview_chop_right(&tmp, offset);
  sview_chop_left(&tmp, start);

  char buffer[tmp.count];
  buffer[tmp.count] = '\0';

  for (int i = 0; i < sizeof(buffer); ++i) {
    buffer[i] = tmp.data[i]; 
  }
  char sample[suffix.count];
  sample[suffix.count] = '\0';
  for (int i = 0; i < sizeof(sample); ++i) {
    sample[i] = buffer[i];
  }

  int result = strcmp(sample, suffix.data);
  
  if (result == 0) {
    return true;
  }

  return false;
}

int find (sview string, sview substr, int start)
{
  int result[32];
  for (int i = start; i < string.count; ++i) {
    if (string.data[i] == substr.data[0]) {
      for (int j = 0; j < substr.count;) {
        //string.data[i + j] == substr.data[j];
        if (string.data[i + j] == substr.data[j]) {
          ++j;
          if (j >= substr.count) {
            return i;
          }
        }
        else {
          return -1;
        }
      }
    }
  }
  return -1;
}


int rfind (sview string, sview substr, int start)
{
  int result[32];
  for (int i = string.count; i > start; --i) {
    if (string.data[i] == substr.data[0]) {
      for (int j = 0; j < substr.count;) {
        //string.data[i + j] == substr.data[j];
        if (string.data[i + j] == substr.data[j]) {
          ++j;
          if (j >= substr.count) {
            return i;
          }
        }
        else {
          return -1;
        }
      }
    }
  }
  return -1;

}
//TODO: 
//        count(x, start, end)
//        find(sub, start, end)
//        rfind(sub, start, end)
//        partition(sv, sep)
//        converter

int main(int argc, char* argv[])
{
  //sview name = sv("Nick");
  //printf("%s, its yours first clean C project, do you like it?\n", name.data);
  //char buffer[64];
  //fgets(buffer, sizeof(buffer),stdin);
  //puts(buffer);
  //sview test_slice = sv("This, and that");
  //sview this = slice(&test_slice, ',');
  //printf("|"STRING_VIEW_FORMATTING"|\n", STRING_VIEW_ARGS(this));
  //sview upper = Upper(name);
  //printf("|"STRING_VIEW_FORMATTING"|\n", STRING_VIEW_ARGS(upper));
  //sview lower = Lower(name);
  //printf("|"STRING_VIEW_FORMATTING"|\n", STRING_VIEW_ARGS(lower));
  //printsv(lower);
  //sview testSentance = sv("this is width string with multiple words that starts with lowercase words and this words will be capitalized after function call!");
  //sview capitalized = capitalize(testSentance);
  //printsv(capitalized);
  //sview titled = title(testSentance);
  //printsv(titled);
  //sview word = sv("uncomfortablylongwordlooooooooooonger");
  //printsv(center(word, 50, '_'));
  //printsv(left(word, 50, '_'));
  //printsv(right(word, 50, '_'));

  //sview qwer = sv("qwer");
  //sview asdf = sv("as");
  //sview zxcv = sv("zxcvbibos");
  //sview zxcv1 = sv("zxcvhaha");
  //sview zxcv2 = sv("zxcvbeniz");
  //sview zxcv3 = sv("zxcv");
  //sview_array_t words = sv_array(&qwer, &asdf, &zxcv, &zxcv1, &zxcv2, &zxcv3);

  //sview joined = join(&words, ","); 
  //printsv(joined);

  //sview src = sv("converTED");
  //////printf("Word:\t%s\nLenght:\t%i\n", src.data, src.count);
  ////char* converted = converter(src, &converted);
  ////printf(converted);
  //sview suffix = sv("TED");
  //bool endresult = endswith(src, suffix, 0, strlen(src.data));
  //printf("%i\n", endresult);

  //sview suffix2 = sv("son");
  //bool startresult = startswith(src, suffix2, 1, strlen(src.data));
  //printf("%i\n", startresult);
  sview findstr = sv("Find some substring");
  sview sub = sv("some");
  int index = find(findstr, sub, 0);
  int r_index = rfind(findstr, sub, 0);
  printf("%i\n", index);
  printf("%i\n", r_index);
  return 0;
}
