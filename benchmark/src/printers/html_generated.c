
fprintf(
    file_handler,
    "\n<html xmlns=\"http://www.w3.org/1999/html\">\n<head>\n	<title>Waverian benchmark results</title>\n	"
    "<style>\n        body {\n            font-family: system-ui, -apple-system, \"Segoe UI\", Roboto, \"Helvetica "
    "Neue\", Arial, \"Noto Sans\", \"Liberation Sans\", sans-serif, \"Apple Color Emoji\", \"Segoe UI Emoji\", \"Segoe "
    "UI Symbol\", \"Noto Color Emoji\"\n        }\n\n        .container {\n            margin-left: auto;\n            "
    "margin-right: auto;\n            min-width: 30em;\n            max-width: 50em;\n            text-align: "
    "center;\n        }\n\n        .thin_font {\n            font-weight: 300;\n            margin: auto;\n        "
    "}\n\n        .white_on_blue {\n            background-color: #1e90ff;\n            color: #f0f8ff;\n        }\n\n "
    "       td {\n            border: 1px solid #1e90ff;\n        }\n\n        h1 {\n            font-size: 5em;\n     "
    "   }\n\n        h2 {\n            font-size: 3em;\n        }\n\n        h3 {\n            font-size: 1.8em;\n     "
    "   }\n\n        h4 {\n            font-size: 1.2em;\n        }\n\n        .row_spacer {\n            border: "
    "none;\n            height: 1.5em;\n        }\n\n        table {\n            border-collapse: collapse;\n         "
    "   border-width: 0;\n            width: 100%%;\n            text-align: left;\n        }\n\n        p {\n         "
    "   text-align: left;\n        }\n\n        .logo {\n            text-align: left;\n            padding: 10px;\n   "
    "     }\n\n        .no_border {\n            border: none;\n        }\n\n        .sub_result_div {\n            "
    "text-align: center;\n            padding-bottom: 10px;\n        }\n	</style>\n</head>\n<body>\n<div "
    "class=\"container\">\n	<div class=\"logo white_on_blue\">\n		<h3 class=\"thin_font\">Waverian "
    "benchmark</h3>\n	</div>\n	<table class=\"table_info\">\n		<thead></thead>\n		"
    "<tbody>\n		<tr>\n			<td>Version</td>\n			<td colspan=\"4\">");
fprintf(file_handler, "%s", parameters.version_info);
fprintf(
    file_handler,
    "</td>\n		</tr>\n		<tr>\n			<td>Date</td>\n			<td colspan=\"4\">");
fprintf(file_handler, "%s", result.timestamp);
fprintf(file_handler,
        "</td>\n		</tr>\n		<tr>\n			<td>Compiler</td>\n			<td "
        "colspan=\"4\">");
fprintf(file_handler, "%s", parameters.compiler_info);
fprintf(
    file_handler,
    "</td>\n		</tr>\n		<tr>\n			<td>Logical cores</td>\n			<td "
    "colspan=\"4\">");
fprintf(file_handler, "%i", result.core_count);
fprintf(file_handler,
        "</td>\n		</tr>\n		<tr>\n			<td>CPU name</td>\n			<td "
        "colspan=\"4\">");
fprintf(file_handler, "%s", parameters.cpu_name);
fprintf(file_handler,
        "</td>\n		</tr>\n		<tr>\n			<td>Comment</td>\n			<td "
        "colspan=\"4\">");
fprintf(file_handler, "%s", result.comment);
fprintf(
    file_handler,
    "</td>\n		</tr>\n		</tbody>\n	</table>\n	<div>\n		<h1 class=\"thin_font\">");
fprintf(file_handler, "%.*f", AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_DISABLED].score));
fprintf(file_handler,
        "\n		</h1>\n		<h3 class=\"thin_font\">Overall score</h3>\n	</div>\n	<br>\n	"
        "<div>\n		<table>\n			<tr>\n				<td "
        "class=\"no_border\">\n					<div class=\"sub_result_div white_on_blue\">\n	"
        "					<h2 class=\"thin_font\">");
if (result.full_result[WB_OPTIMIZATION_DISABLED].detailed[WB_RUN_TYPE_SINGLE_CORE].valid) {
  fprintf(file_handler, "%.*f",
          AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_DISABLED].detailed[WB_RUN_TYPE_SINGLE_CORE].score));
} else {
  fprintf(file_handler, "N/A");
}
fprintf(file_handler,
        "</h2>\n						<h4 class=\"thin_font\">-</h4>\n		"
        "				<h3 class=\"thin_font\">Singlecore</h3>\n				"
        "	</div>\n				</td>\n				<td "
        "class=\"no_border\">\n					<div class=\"sub_result_div white_on_blue\">\n	"
        "					<h2 class=\"thin_font\">");
if (result.full_result[WB_OPTIMIZATION_DISABLED].detailed[WB_RUN_TYPE_MULTI_CORE].valid) {
  fprintf(file_handler, "%.*f",
          AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_DISABLED].detailed[WB_RUN_TYPE_MULTI_CORE].score));
} else {
  fprintf(file_handler, "N/A");
}
fprintf(file_handler, "</h2>\n						<h4 class=\"thin_font\">");
if (result.full_result[WB_OPTIMIZATION_DISABLED].detailed[WB_RUN_TYPE_MULTI_CORE].valid) {
  fprintf(file_handler, "%.1f", result.full_result[WB_OPTIMIZATION_DISABLED].detailed[WB_RUN_TYPE_MULTI_CORE].ratio);
  fprintf(file_handler, " x ratio");
} else {
  fprintf(file_handler, " -");
}
fprintf(
    file_handler,
    "</h4>\n						<h3 class=\"thin_font\">Multicore</h3>\n		"
    "			</div>\n				</td>\n			</tr>\n			"
    "<tr>\n				<td class=\"no_border\">\n					<div "
    "class=\"sub_result_div white_on_blue\">\n						<h2 class=\"thin_font\">");
if (result.full_result[WB_OPTIMIZATION_DISABLED].detailed[WB_RUN_TYPE_QUAD_CORE].valid) {
  fprintf(file_handler, "%.*f",
          AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_DISABLED].detailed[WB_RUN_TYPE_QUAD_CORE].score));
} else {
  fprintf(file_handler, "N/A");
}
fprintf(file_handler, "</h2>\n						<h4 class=\"thin_font\">");
if (result.full_result[WB_OPTIMIZATION_DISABLED].detailed[WB_RUN_TYPE_QUAD_CORE].valid) {
  fprintf(file_handler, "%.1f", result.full_result[WB_OPTIMIZATION_DISABLED].detailed[WB_RUN_TYPE_QUAD_CORE].ratio);
  fprintf(file_handler, " x ratio");
} else {
  fprintf(file_handler, " -");
}
fprintf(file_handler,
        "</h4>\n						<h3 class=\"thin_font\">Quadcore</h3>\n		"
        "			</div>\n				</td>\n				<td "
        "class=\"no_border\">\n					<div class=\"sub_result_div white_on_blue\">\n	"
        "					<h2 class=\"thin_font\">");
if (result.full_result[WB_OPTIMIZATION_DISABLED].detailed[WB_RUN_TYPE_WORKSTATION].valid) {
  fprintf(file_handler, "%.*f",
          AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_DISABLED].detailed[WB_RUN_TYPE_WORKSTATION].score));
} else {
  fprintf(file_handler, "N/A");
}
fprintf(file_handler, "</h2>\n						<h4 class=\"thin_font\">");
if (result.full_result[WB_OPTIMIZATION_DISABLED].detailed[WB_RUN_TYPE_WORKSTATION].valid) {
  fprintf(file_handler, "%.1f", result.full_result[WB_OPTIMIZATION_DISABLED].detailed[WB_RUN_TYPE_WORKSTATION].ratio);
  fprintf(file_handler, " x ratio");
} else {
  fprintf(file_handler, " -");
}
fprintf(file_handler,
        "</h4>\n						<h3 class=\"thin_font\">Workstation</h3>\n	"
        "				</div>\n				</td>\n			</tr>\n	"
        "	</table>\n	</div>\n	<div>\n		<table>\n			<tr "
        "class=\"row_spacer\"></tr>\n			<tr class=\"white_on_blue\">\n				"
        "<td></td>\n				<td><b>Non optimized</b></td>");
for (j = 0; j < WB_RUN_TYPE_SIZE; j++) {
  if (result.full_result[WB_OPTIMIZATION_DISABLED].detailed[j].valid) {
    fprintf(file_handler, "\n				<td>");
    fprintf(file_handler, "%s", WB_RUN_TYPE_NAMES[j]);
    fprintf(file_handler, "</td>");
  }
}
fprintf(file_handler,
        "\n			</tr>\n			<tr>\n				<td></td>\n		"
        "		<td>Maximum</td>");
for (j = 0; j < WB_RUN_TYPE_SIZE; j++) {
  if (result.full_result[WB_OPTIMIZATION_DISABLED].detailed[j].valid) {
    fprintf(file_handler, "\n				<td>");
    fprintf(file_handler, "%.*f", AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_DISABLED].detailed[j].maximum));
    fprintf(file_handler, "</td>");
  }
}
fprintf(file_handler,
        "\n			</tr>\n			<tr>\n				<td></td>\n		"
        "		<td>Average</td>");
for (j = 0; j < WB_RUN_TYPE_SIZE; j++) {
  if (result.full_result[WB_OPTIMIZATION_DISABLED].detailed[j].valid) {
    fprintf(file_handler, "\n				<td>");
    fprintf(file_handler, "%.*f", AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_DISABLED].detailed[j].average));
    fprintf(file_handler, "</td>");
  }
}
fprintf(file_handler,
        "\n			</tr>\n			<tr>\n				<td></td>\n		"
        "		<td>Geometric</td>");
for (j = 0; j < WB_RUN_TYPE_SIZE; j++) {
  if (result.full_result[WB_OPTIMIZATION_DISABLED].detailed[j].valid) {
    fprintf(file_handler, "\n				<td>");
    fprintf(file_handler, "%.*f", AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_DISABLED].detailed[j].geometric));
    fprintf(file_handler, "</td>");
  }
}
fprintf(file_handler,
        "\n			</tr>\n			<tr>\n				<td></td>\n		"
        "		<td>Harmonic</td>");
for (j = 0; j < WB_RUN_TYPE_SIZE; j++) {
  if (result.full_result[WB_OPTIMIZATION_DISABLED].detailed[j].valid) {
    fprintf(file_handler, "\n				<td>");
    fprintf(file_handler, "%.*f", AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_DISABLED].detailed[j].harmonic));
    fprintf(file_handler, "</td>");
  }
}
fprintf(file_handler,
        "\n			</tr>\n			<tr>\n				<td></td>\n		"
        "		<td>Minimum</td>");
for (j = 0; j < WB_RUN_TYPE_SIZE; j++) {
  if (result.full_result[WB_OPTIMIZATION_DISABLED].detailed[j].valid) {
    fprintf(file_handler, "\n				<td>");
    fprintf(file_handler, "%.*f", AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_DISABLED].detailed[j].minimum));
    fprintf(file_handler, "</td>");
  }
}
fprintf(file_handler,
        "\n			</tr>\n			<tr class=\"white_on_blue\">\n				"
        "<td>#</td>\n				<td>Kernel name</td>");
for (j = 0; j < WB_RUN_TYPE_SIZE; j++) {
  if (result.full_result[WB_OPTIMIZATION_DISABLED].detailed[j].valid) {
    fprintf(file_handler, "\n				<td></td>");
  }
}
fprintf(file_handler, "\n			</tr>");
for (i = 0; i < WB_KERNEL_COUNT; i++) {
  fprintf(file_handler, "\n			<tr>\n				<td>");
  fprintf(file_handler, "%i", i + 1);
  fprintf(file_handler, ".</td>\n				<td>");
  fprintf(file_handler, "%s", WB_KERNEL_NAMES[i]);
  fprintf(file_handler, "</td>");
  for (j = 0; j < WB_RUN_TYPE_SIZE; j++) {
    if (result.full_result[WB_OPTIMIZATION_DISABLED].detailed[j].valid) {
      fprintf(file_handler, "\n				<td>");
      fprintf(file_handler, "%.*f",
              AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_DISABLED].detailed[j].kernel_results[i]));
      fprintf(file_handler, "</td>");
    }
  }
  fprintf(file_handler, "\n			</tr>");
}
fprintf(file_handler, "\n		</table>\n		<p>\n			Optimized score - ");
if (result.full_result[WB_OPTIMIZATION_ENABLED].score) {
  fprintf(file_handler, "%.*f", AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_ENABLED].score));
} else {
  fprintf(file_handler, "N/A");
}
fprintf(file_handler, "\n			(");
if (result.full_result[WB_OPTIMIZATION_ENABLED].score && result.full_result[WB_OPTIMIZATION_DISABLED].score) {
  fprintf(file_handler, "%.1f",
          result.full_result[WB_OPTIMIZATION_ENABLED].score / result.full_result[WB_OPTIMIZATION_DISABLED].score);
  fprintf(file_handler, " x optimized ratio)");
}
fprintf(file_handler,
        "\n		</p>\n		<table>\n			<tr class=\"white_on_blue\">\n		"
        "		<td></td>\n				<td><b>Optimized</b></td>");
for (j = 0; j < WB_RUN_TYPE_SIZE; j++) {
  if (result.full_result[WB_OPTIMIZATION_ENABLED].detailed[j].valid) {
    fprintf(file_handler, "\n				<td>");
    fprintf(file_handler, "%s", WB_RUN_TYPE_NAMES[j]);
    fprintf(file_handler, "</td>");
  }
}
fprintf(file_handler,
        "\n			</tr>\n			<tr>\n				<td></td>\n		"
        "		<td>Maximum</td>");
for (j = 0; j < WB_RUN_TYPE_SIZE; j++) {
  if (result.full_result[WB_OPTIMIZATION_ENABLED].detailed[j].valid) {
    fprintf(file_handler, "\n				<td>");
    fprintf(file_handler, "%.*f", AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_ENABLED].detailed[j].maximum));
    fprintf(file_handler, "</td>");
  }
}
fprintf(file_handler,
        "\n			</tr>\n			<tr>\n				<td></td>\n		"
        "		<td>Average</td>");
for (j = 0; j < WB_RUN_TYPE_SIZE; j++) {
  if (result.full_result[WB_OPTIMIZATION_ENABLED].detailed[j].valid) {
    fprintf(file_handler, "\n				<td>");
    fprintf(file_handler, "%.*f", AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_ENABLED].detailed[j].average));
    fprintf(file_handler, "</td>");
  }
}
fprintf(file_handler,
        "\n			</tr>\n			<tr>\n				<td></td>\n		"
        "		<td>Geometric</td>");
for (j = 0; j < WB_RUN_TYPE_SIZE; j++) {
  if (result.full_result[WB_OPTIMIZATION_ENABLED].detailed[j].valid) {
    fprintf(file_handler, "\n				<td>");
    fprintf(file_handler, "%.*f", AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_ENABLED].detailed[j].geometric));
    fprintf(file_handler, "</td>");
  }
}
fprintf(file_handler,
        "\n			</tr>\n			<tr>\n				<td></td>\n		"
        "		<td>Harmonic</td>");
for (j = 0; j < WB_RUN_TYPE_SIZE; j++) {
  if (result.full_result[WB_OPTIMIZATION_ENABLED].detailed[j].valid) {
    fprintf(file_handler, "\n				<td>");
    fprintf(file_handler, "%.*f", AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_ENABLED].detailed[j].harmonic));
    fprintf(file_handler, "</td>");
  }
}
fprintf(file_handler,
        "\n			</tr>\n			<tr>\n				<td></td>\n		"
        "		<td>Minimum</td>");
for (j = 0; j < WB_RUN_TYPE_SIZE; j++) {
  if (result.full_result[WB_OPTIMIZATION_ENABLED].detailed[j].valid) {
    fprintf(file_handler, "\n				<td>");
    fprintf(file_handler, "%.*f", AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_ENABLED].detailed[j].minimum));
    fprintf(file_handler, "</td>");
  }
}
fprintf(file_handler,
        "\n			</tr>\n\n			<tr class=\"white_on_blue\">\n			"
        "	<td>#</td>\n				<td>Kernel name</td>");
for (j = 0; j < WB_RUN_TYPE_SIZE; j++) {
  if (result.full_result[WB_OPTIMIZATION_DISABLED].detailed[j].valid) {
    fprintf(file_handler, "\n				<td></td>");
  }
}
fprintf(file_handler, "\n			</tr>");
for (i = 0; i < WB_KERNEL_COUNT; i++) {
  fprintf(file_handler, "\n			<tr>\n				<td>");
  fprintf(file_handler, "%i", i + 1);
  fprintf(file_handler, ".</td>\n				<td>");
  fprintf(file_handler, "%s", WB_KERNEL_NAMES[i]);
  fprintf(file_handler, "</td>");
  for (j = 0; j < WB_RUN_TYPE_SIZE; j++) {
    if (result.full_result[WB_OPTIMIZATION_ENABLED].detailed[j].valid) {
      fprintf(file_handler, "\n				<td>");
      fprintf(file_handler, "%.*f",
              AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_ENABLED].detailed[j].kernel_results[i]));
      fprintf(file_handler, "</td>");
    }
  }
  fprintf(file_handler, "\n			</tr>");
}
fprintf(file_handler, "\n		</table>\n	</div>\n</div>\n</body>\n</html>");
