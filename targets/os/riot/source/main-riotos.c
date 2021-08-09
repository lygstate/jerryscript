/* Copyright JS Foundation and other contributors, http://js.foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jerryscript-port.h"
#include "jerryscript.h"

#include "jerryscript-ext/handler.h"
#include "shell.h"

/**
 * Standalone Jerry exit codes
 */
#define JERRY_STANDALONE_EXIT_CODE_OK   (0)
#define JERRY_STANDALONE_EXIT_CODE_FAIL (1)

/**
 * Register a JavaScript function in the global object.
 */
static void
register_js_function (const char *name_p, /**< name of the function */
                      jerry_external_handler_t handler_p) /**< function callback */
{
  jerry_value_t result_val = jerryx_handler_register_global (name_p, handler_p);

  if (jerry_value_is_exception (result_val))
  {
    printf ("Warning: failed to register '%s' method.", name_p);
  }

  jerry_value_free (result_val);
} /* register_js_function */

/**
 * Jerryscript simple test
 */
int
test_jerry (int argc, char **argv)
{
  /* Suppress compiler errors */
  (void) argc;
  (void) argv;

  jerry_value_t ret_value = jerry_undefined ();

  const jerry_char_t script[] = "print ('Hello, World!');";
  printf ("This test run the following script code: [%s]\n\n", script);

  /* Initialize engine */
  jerry_init (JERRY_INIT_EMPTY);

  /* Register the print function in the global object. */
  register_js_function ("print", jerryx_handler_print);

  /* Setup Global scope code */
  ret_value = jerry_parse (script, sizeof (script) - 1, NULL);

  if (!jerry_value_is_exception (ret_value))
  {
    /* Execute the parsed source code in the Global scope */
    ret_value = jerry_run (ret_value);
  }

  int ret_code = JERRY_STANDALONE_EXIT_CODE_OK;

  if (jerry_value_is_exception (ret_value))
  {
    printf ("Script Error!");

    ret_code = JERRY_STANDALONE_EXIT_CODE_FAIL;
  }

  jerry_value_free (ret_value);

  /* Cleanup engine */
  jerry_cleanup ();

  return ret_code;

} /* test_jerry */

const shell_command_t shell_commands[] = { { "test", "Jerryscript Hello World test", test_jerry },
                                           { NULL, NULL, NULL } };

char*
getcwd (char* _DstBuf, int _SizeInBytes)
{
  if (_DstBuf != NULL)
  {
    if (_SizeInBytes < 2)
    {
      return NULL;
    }
  }
  else
  {
    _DstBuf = malloc (2);
  }
  strcpy (_DstBuf, "/");
  return _DstBuf;
}

int
main (void)
{
  union
  {
    double d;
    unsigned u;
  } now = { .d = jerry_port_get_current_time () };
  srand (now.u);
  printf ("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
  printf ("This board features a(n) %s MCU.\n", RIOT_MCU);

  /* start the shell */
  char line_buf[SHELL_DEFAULT_BUFSIZE];
  shell_run (shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

  return 0;
}
