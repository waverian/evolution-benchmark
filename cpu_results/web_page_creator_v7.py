#!/usr/bin/env python3
#
# Copyright (c) 2022 Waverian. All rights reserved.
# (Updated 2026: Added Opt/Non-Opt parsing, Legacy Browser Support, JS Natural Sort, and robust Error Handling)
#
# This script takes entire folder of results (in TXT), and generates a webpage HTML as an index, where it shows
#  all the CPU models, single-core and multi-core results.
#

import argparse
import re
import webbrowser
import logging
import sys
import os
from typing import Optional

def setup_logger(verbosity: int = None, log_file: Optional[str] = None) -> None:
    """"Set up the logger with specified verbosity level."""
    handlers = []
    stdout_level = {
        0: logging.WARN,
        1: logging.INFO,
        2: logging.DEBUG
    }.get(min(verbosity, 2), logging.WARN)

    stdout_handler = logging.StreamHandler(sys.stdout)
    stdout_handler.setLevel(stdout_level)
    stdout_formatter = logging.Formatter('%(levelname)s: %(message)s')
    stdout_handler.setFormatter(stdout_formatter)
    handlers.append(stdout_handler)

    if log_file:
        file_handler = logging.FileHandler(log_file)
        file_handler.setLevel(logging.DEBUG)
        file_formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s', datefmt='%Y-%m-%d %H:%M:%S')
        file_handler.setFormatter(file_formatter)
        handlers.append(file_handler)

    logging.basicConfig(level=logging.NOTSET, handlers=handlers)

def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description='Web Page Creator Tool')
    parser.add_argument('path', type=str, nargs='?', default='.', help='Input path to the folder with benchmark results')
    parser.add_argument('--recursive', action='store_true', help='Enable recursive walk mode')
    parser.add_argument('--sort', '-s', help='Specify sorting type: "st" or "mt". Defaults to "st".', type=str, choices=['st', 'mt'], default='st')
    logging_group = parser.add_argument_group('logging options')
    logging_group.add_argument('-v', '--verbose', help='Increase verbosity (use -v, -vv)', action='count', default=0, dest='log_level')
    logging_group.add_argument('-l', '--log_file', help='Set path to output log file', action='store', default=None)
    return parser.parse_args()

def read_file(filepath):
    encodings = ["utf-8", "cp1252", "iso-8859-1"]
    for enc in encodings:
        try:
            with open(filepath, "r", encoding=enc, errors="replace") as file:
                return file.read()
        except UnicodeDecodeError:
            continue
    raise ValueError(f"File encoding could not be determined for {filepath}")

def app_main(args: argparse.Namespace) -> int:
    path = os.path.abspath(os.path.expanduser(args.path))
    logging.info(f'Base path is {path}')
    if not os.path.isdir(path):
        raise RuntimeError(f'Input path {path} is not a valid directory')

    all_files = []
    for dirpath, _, filenames in os.walk(path):
        for filename in filenames:
            all_files.append(os.path.join(dirpath, filename))
        if not args.recursive:
            break

    logging.debug(f'Changed working directory to {path}')
    os.chdir(path)

    # 1. Group files by report name
    reports = dict()
    duplicated_report_names = dict()
    for file_full_path in all_files:
        file_name = os.path.basename(file_full_path)
        report_name = os.path.splitext(file_name)[0]
        extension = os.path.splitext(file_name)[1].lower()

        if extension == '.txt':
            key = 'text'
        elif extension in ['.html', '.htm']:
            key = 'html'
        else:
            continue

        if report_name in duplicated_report_names:
            duplicated_report_names[report_name].add(file_full_path)
            continue

        if report_name not in reports:
            reports[report_name] = {}

        if key in reports[report_name]:
            duplicated_report_names[report_name] = set(reports[report_name].values())
            duplicated_report_names[report_name].add(file_full_path)
            del reports[report_name]
            continue

        reports[report_name][key] = os.path.relpath(file_full_path)

    # 2. Filter out incomplete pairs (missing txt or html)
    reports_to_delete = set()
    for report_name, report in reports.items():
        if 'text' not in report or 'html' not in report:
            logging.info(f'{report_name} missing text or html pair. Ignoring.')
            reports_to_delete.add(report_name)

    for report_name in reports_to_delete:
        del reports[report_name]

    # 3. Data Extraction phase
    reports_to_delete = set()
    for report_name, data in reports.items():
        logging.debug(f'Processing report: {report_name}')
        txt_file = data['text']
        report_text = read_file(txt_file)

        # Safe regex matching for CPU Name
        cpu_name_match = re.search(r'(?i)CPU name\s*(?:\r?\n)?\s*-\s*([^\n]+)', report_text)
        if not cpu_name_match:
            logging.error(f"Skipping {txt_file}: Missing CPU name.")
            reports_to_delete.add(report_name)
            continue
        
        data['cpu_name'] = cpu_name_match.group(1).strip()

        # Split into Optimized and Non-Optimized blocks
        parts = re.split(r'(?i)Optimized score\s*-', report_text)
        if len(parts) < 2:
            logging.error(f"Skipping {txt_file}: Could not find both Optimized and Non-Optimized sections.")
            reports_to_delete.add(report_name)
            continue

        non_opt_text, opt_text = parts[0], parts[1]

        # Helper to safely extract single/multi values
        def extract_scores(text_block):
            st = re.search(r'(?i)Single core\s*-\s*([\d\.]+)', text_block)
            mt = re.search(r'(?i)Multi core\s*-\s*([\d\.]+)', text_block)
            return {
                'st': float(st.group(1)) if st else 0.0,
                'mt': float(mt.group(1)) if mt else 0.0
            }

        data['non_opt'] = extract_scores(non_opt_text)
        data['opt'] = extract_scores(opt_text)

        if data['opt']['st'] == 0.0 or data['non_opt']['st'] == 0.0:
            logging.error(f"Skipping {txt_file}: Missing valid score data.")
            reports_to_delete.add(report_name)
            continue

    for report_name in reports_to_delete:
        del reports[report_name]

    if not reports:
        logging.error("No valid reports found to process! Exiting.")
        return 1

    # 4. Prepare dictionaries for Opt and Non-Opt output
    opt_data = {}
    non_opt_data = {}
    for name, data in reports.items():
        opt_data[name] = {'cpu_name': data['cpu_name'], 'st_result': data['opt']['st'], 'mt_result': data['opt']['mt'], 'text': data['text'], 'html': data['html']}
        non_opt_data[name] = {'cpu_name': data['cpu_name'], 'st_result': data['non_opt']['st'], 'mt_result': data['non_opt']['mt'], 'text': data['text'], 'html': data['html']}

    # Output HTML files
    logging.info("Generating webpage_index_Optimized.html")
    with open('webpage_index_Optimized.html', 'w+', encoding='utf-8') as file:
        py_print_html(file, opt_data, "Optimized")

    logging.info("Generating webpage_index_NonOptimized.html")
    with open('webpage_index_NonOptimized.html', 'w+', encoding='utf-8') as file:
        py_print_html(file, non_opt_data, "Non-Optimized")

    webbrowser.open('webpage_index_Optimized.html')
    return 0

def py_print_html(file, reports, title_suffix):
    # Embedded HTML with strictly ES3-compliant JS for legacy browser compatibility
    html_template = f"""<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
    <meta charset="utf-8">
    <title>Waverian Evolution - {title_suffix}</title>
    <style>
        body {{ font-family: Arial, sans-serif; }}
        .container {{ margin: 0 auto; min-width: 30em; max-width: 60em; text-align: center; }}
        .thin_font {{ font-weight: 300; margin: auto; }}
        .white_on_blue {{ background-color: #1e90ff; color: #f0f8ff; }}
        table.blue_border td {{ border: 1px solid #1e90ff; padding: 4px; }}
        th {{ cursor: pointer; padding: 6px; }}
        h3 {{ font-size: 1.8em; }}
        table {{ border-collapse: collapse; border-width: 0; width: 100%; text-align: left; }}
        .logo {{ text-align: left; padding: 10px; }}
        .sort-indicator {{ font-size: 0.8em; margin-left: 5px; color: #ccffff; }}
    </style>
    <script type="text/javascript">
        window.onload = function() {{
            var table = document.getElementById("score_table");
            if (!table) return;
            var headers = table.getElementsByTagName("th");
            var tbody = table.getElementsByTagName("tbody")[0];
            
            // Set default sort to Name (index 0) in Ascending order
            var currentSort = {{ index: 0, direction: "asc" }}; 

            function getText(node) {{
                return node.innerText || node.textContent || "";
            }}

            // Natural sorting chunker for legacy browsers (splits into letters and numbers)
            function chunkify(t) {{
                var tz = [];
                var x = 0, y = -1, n = 0, i, j;
                while (i = (j = t.charAt(x++)).charCodeAt(0)) {{
                    var m = (i >= 48 && i <= 57); // Is it a digit 0-9?
                    if (m !== n) {{
                        tz[++y] = "";
                        n = m;
                    }}
                    tz[y] += j;
                }}
                return tz;
            }}

            function sortTable(index, direction, type) {{
                var rows = tbody.getElementsByTagName("tr");
                var rowsArray = [];
                for (var i = 0; i < rows.length; i++) {{
                    rowsArray.push(rows[i]);
                }}

                rowsArray.sort(function(a, b) {{
                    var aCells = a.getElementsByTagName("td");
                    var bCells = b.getElementsByTagName("td");
                    if (aCells.length <= index || bCells.length <= index) return 0;

                    var aText = getText(aCells[index]).replace(/^\\s+|\\s+$/g, '');
                    var bText = getText(bCells[index]).replace(/^\\s+|\\s+$/g, '');

                    var cmp = 0;
                    if (type === "number") {{
                        var aNum = parseFloat(aText);
                        var bNum = parseFloat(bText);
                        if (aNum < bNum) cmp = -1;
                        else if (aNum > bNum) cmp = 1;
                    }} else {{
                        // Execute Natural Sort for Strings
                        var aa = chunkify(aText);
                        var bb = chunkify(bText);
                        for (var x = 0; aa[x] && bb[x]; x++) {{
                            if (aa[x] !== bb[x]) {{
                                var c = Number(aa[x]);
                                var d = Number(bb[x]);
                                if (!isNaN(c) && !isNaN(d)) {{
                                    cmp = c - d;
                                    break;
                                }} else {{
                                    var aStr = String(aa[x]).toLowerCase();
                                    var bStr = String(bb[x]).toLowerCase();
                                    if (aStr < bStr) cmp = -1;
                                    else if (aStr > bStr) cmp = 1;
                                    break;
                                }}
                            }}
                        }}
                        if (cmp === 0) cmp = aa.length - bb.length;
                    }}
                    return direction === "asc" ? cmp : -cmp;
                }});

                while (tbody.firstChild) {{
                    tbody.removeChild(tbody.firstChild);
                }}
                for (var j = 0; j < rowsArray.length; j++) {{
                    tbody.appendChild(rowsArray[j]);
                }}
            }}

            function updateIndicators(index, direction) {{
                for (var i = 0; i < headers.length; i++) {{
                    var spans = headers[i].getElementsByTagName("span");
                    if (spans.length > 0) {{
                        spans[0].innerHTML = "&#9666;"; // Left pointing triangle
                        if (i === index) {{
                            spans[0].innerHTML = direction === "asc" ? "&#9650;" : "&#9660;"; 
                        }}
                    }}
                }}
            }}

            for (var i = 0; i < headers.length; i++) {{
                (function(idx) {{
                    var header = headers[idx];
                    var type = header.getAttribute("data-type");
                    if (!type) return;

                    header.onclick = function() {{
                        var direction = (currentSort.index === idx && currentSort.direction === "asc") ? "desc" : "asc";
                        currentSort = {{ index: idx, direction: direction }};
                        sortTable(idx, direction, type);
                        updateIndicators(idx, direction);
                    }};
                }})(i);
            }}

            // Trigger the default natural sort on load
            var defaultType = headers[currentSort.index].getAttribute("data-type");
            sortTable(currentSort.index, currentSort.direction, defaultType);
            updateIndicators(currentSort.index, currentSort.direction);
        }};
    </script>
</head>
<body>
<div class="container">
    <div class="logo white_on_blue">
        <h3 class="thin_font">Waverian Evolution benchmark ({title_suffix})</h3>
    </div>
    <br />
    <table class="blue_border" id="score_table">
        <thead class="white_on_blue">
            <tr>
                <th data-type="string">Name<span class="sort-indicator">&#9666;</span></th>
                <th data-type="number">Score ST<span class="sort-indicator">&#9666;</span></th>
                <th data-type="number">Score MT<span class="sort-indicator">&#9666;</span></th>
                <th>Text</th>
                <th>Html</th>
            </tr>
        </thead>
        <tbody>
"""
    file.write(html_template)

    for name, report in reports.items():
        st_val = report["st_result"]
        mt_val = report["mt_result"]
        file.write('            <tr>\n')
        file.write(f'                <td>{report["cpu_name"]}</td>\n')
        file.write(f'                <td>{st_val:.2f}</td>\n')
        file.write(f'                <td>{mt_val:.2f}</td>\n')
        file.write(f'                <td><a href="{report["text"]}">Text</a></td>\n')
        file.write(f'                <td><a href="{report["html"]}">Html</a></td>\n')
        file.write('            </tr>\n')

    file.write("""        </tbody>
    </table>
</div>
</body>
</html>
""")

def main():
    __arguments = parse_arguments()
    setup_logger(__arguments.log_level, __arguments.log_file)
    __error_code = 0
    try:
        logging.debug(f'ENTER main with command line arguments: {__arguments.__dict__}')
        __result = app_main(__arguments)
        __error_code = __result if isinstance(__result, int) else 0
    except Exception as e:
        logging.fatal('Fatal error occurred:', exc_info=True)
        __error_code = 1
    finally:
        logging.debug(f'EXIT main with error code {__error_code}')
        sys.exit(__error_code)

if __name__ == '__main__':
    main()
