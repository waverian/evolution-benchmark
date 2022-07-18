/*
 * Copyright (c) 2022 waverian
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "html_report_generator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lfk/constants.h"

const char *html_template =
    "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>LFK "
    "Benchmark "
    "result</title><style>body{font-family:system-ui,-apple-system,\"Segoe "
    "UI\",Roboto,\"Helvetica Neue\",Arial,\"Noto Sans\",\"Liberation "
    "Sans\",sans-serif,\"Apple Color Emoji\",\"Segoe UI Emoji\",\"Segoe UI "
    "Symbol\",\"Noto Color "
    "Emoji\"}.container{margin-left:auto;margin-right:auto;min-width:30em;max-"
    "width:50em}h1{font-weight:300;line-height:1.2}.container>h1{font-size:3em;"
    "background-color:#1e90ff;color:#f0f8ff}.overall_results{text-align:center}"
    ".overall_results "
    "h1{font-size:8em;margin:0}table{border-collapse:collapse;border-width:0;"
    "width:100%%}tr>td{border-bottom:1px solid "
    "#000}thead{background-color:#1e90ff;color:#f0f8ff}</style></"
    "head><body><div class=\"container\"><h1 class=\"header\">LFK Benchmark "
    "result</h1><table class=\"overall_results\"><!--PLACEHOLDER-->%s        "
    "<!--PLACEHOLDER--></table><table "
    "class=\"system-information\"><thead></thead><!--PLACEHOLDER-->%s        "
    "<!--PLACEHOLDER--><!--PLACEHOLDER-->%s        "
    "<!--PLACEHOLDER--><!--PLACEHOLDER-->%s        "
    "<!--PLACEHOLDER--></table></div></body></html>";
static void _generate_report(const char *file_path,
                             const char *overall_result_table,
                             const char *system_param_table,
                             const char *result_table,
                             const char *detailed_result_table) {
  FILE *file_handler;
  file_handler = fopen(file_path, "w");

  fprintf(file_handler, html_template, overall_result_table, system_param_table,
          result_table, detailed_result_table);
  fclose(file_handler);
}

static char *create_overall_result_table(const lfk_full_result_t *result_p) {
  const char *format = ""
                       "<tbody>"
                       "<tr>"
                       "<td>"
                       "<h1>%7.0f</h1>" /* Singlecore result placeholder*/
                       "</td>"
                       "<td>"
                       "<h1>%7.0f</h1>" /* Multicore result placeholder*/
                       "</td>"
                       "</tr>"
                       "<tr>"
                       "<td>"
                       "<h4>Singlecore</h4>"
                       "</td>"
                       "<td>"
                       "<h4>Multicore</h4>"
                       "</td>"
                       "</tr>"
                       "</tbody>";

  char buffer[1024] = "";
  snprintf(buffer, 1024, format, result_p->singlecore_result,
           result_p->multicore_result);
  return strdup(buffer);
}
static char *create_system_param_table(const benchmark_handler_t result_p) {
  const char *format = ""
                       "<tbody>"
                       "<tr>"
                       "<td>%s</td>"
                       "<td colspan=\"4\">%s</td>"
                       "</tr>";

  char buffer[10240] = "";
  int len = 0;

  char core_count_as_char[10];
  snprintf(core_count_as_char, sizeof(core_count_as_char), "%i",
           benchmark_get_core_count(result_p));

  len += snprintf(buffer + len, 10240 - len, format, "Version",
                  benchmark_get_version(result_p));
  len += snprintf(buffer + len, 10240 - len, format, "Date",
                  benchmark_get_date(result_p));
  len += snprintf(buffer + len, 10240 - len, format, "Compiler",
                  benchmark_get_compiler_info(result_p));
  len += snprintf(buffer + len, 10240 - len, format, "Core count",
                  core_count_as_char);
  len += snprintf(buffer + len, 10240 - len, format, " ", " ");
  len += snprintf(buffer + len, 10240 - len, "</tbody>");

  return strdup(buffer);
}

static char *create_result_table(const lfk_full_result_t *result_p) {
  char buffer[4196] = ""
                      "<thead>"
                      "<tr>"
                      "<th>MFLOPS</th>"
                      "<th>Singlecore Non Optimized</th>"
                      "<th>Multicore Non Optimized</th>"
                      "<th>Singlecore Optimized</th>"
                      "<th>Multicore Optimized</th>"
                      "</tr>"
                      "</thead>"
                      "<tbody>";

  const char *format = ""
                       "<tr>"
                       "<td>%s</td>"
                       "<td>%7.0f</td>"
                       "<td>%7.0f</td>"
                       "<td>%7.0f</td>"
                       "<td>%7.0f</td>"
                       "</tr>";

  int len = (int)strlen(buffer);

  len += snprintf(
      buffer + len, 4196 - len, format, "Maximum",
      result_p->lfk_run_result[0].maximum, result_p->lfk_run_result[1].maximum,
      result_p->lfk_run_result[2].maximum, result_p->lfk_run_result[3].maximum);
  len += snprintf(
      buffer + len, 4196 - len, format, "Average",
      result_p->lfk_run_result[0].average, result_p->lfk_run_result[1].average,
      result_p->lfk_run_result[2].average, result_p->lfk_run_result[3].average);
  len += snprintf(buffer + len, 4196 - len, format, "Geometric",
                  result_p->lfk_run_result[0].geometric,
                  result_p->lfk_run_result[1].geometric,
                  result_p->lfk_run_result[2].geometric,
                  result_p->lfk_run_result[3].geometric);
  len += snprintf(buffer + len, 4196 - len, format, "Harmonic",
                  result_p->lfk_run_result[0].harmonic,
                  result_p->lfk_run_result[1].harmonic,
                  result_p->lfk_run_result[2].harmonic,
                  result_p->lfk_run_result[3].harmonic);
  len += snprintf(
      buffer + len, 4196 - len, format, "Minimum",
      result_p->lfk_run_result[0].minimum, result_p->lfk_run_result[1].minimum,
      result_p->lfk_run_result[2].minimum, result_p->lfk_run_result[3].minimum);
  len += snprintf(buffer + len, 4196 - len, "</tbody>");
  return strdup(buffer);
}
static char *create_detailed_result_table(const lfk_full_result_t *result_p) {
  const char *format = ""
                       "<tr>"
                       "<td>%s</td>"
                       "<td>%7.0f</td>"
                       "<td>%7.0f</td>"
                       "<td>%7.0f</td>"
                       "<td>%7.0f</td>"
                       "</tr>";

  char buffer[10240] = ""
                       "<thead>"
                       "<tr>"
                       "<th>MFLOPS</th>"
                       "<th>Singlecore Non Optimized</th>"
                       "<th>Multicore Non Optimized</th>"
                       "<th>Singlecore Optimized</th>"
                       "<th>Multicore Optimized</th>"
                       "</tr>"
                       "</thead>"
                       "<tbody>";

  int len = (int)strlen(buffer);
  int i;

  for (i = 0; i < 24; i++) {
    len += snprintf(buffer + len, 10240 - len, format, KERNEL_NAMES[i],
                    result_p->lfk_run_result[0].kernel_results[i],
                    result_p->lfk_run_result[1].kernel_results[i],
                    result_p->lfk_run_result[2].kernel_results[i],
                    result_p->lfk_run_result[3].kernel_results[i]);
  }
  len += snprintf(buffer + len, 10240 - len, "</tbody>");

  return strdup(buffer);
}

void html_report_generator_create_report(benchmark_handler_t handler,
                                         const char *file_path) {
  const lfk_full_result_t *result = benchmark_get_results(handler);

  char *overall_result_table = create_overall_result_table(result);
  char *system_param_table = create_system_param_table(handler);
  char *result_table = create_result_table(result);
  char *detailed_result_table = create_detailed_result_table(result);

  _generate_report(file_path, overall_result_table, system_param_table,
                   result_table, detailed_result_table);

  free(overall_result_table);
  free(system_param_table);
  free(result_table);
  free(detailed_result_table);
}
