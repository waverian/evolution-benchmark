# This script Generates CPU Architecture Analysis report in HTML from all *.txt file results from EvoBench.
#
# Script goals for Gemini AI Pro:
# ---
# I have a lot of benchmark results of different CPUs. About 200 models in total. Results were acquired via an App called "EvoBench", which is a multi-core version of the ancient Livermore Loops benchmark, and also compatible with modern phones, ported to Android and IOS.
# Here I have uploaded 3 results as examples, in plain text files.
# Note that some files have critical keywords, such as "Clockrate" and "TDP", and some are missing this critical information. You should skip the files that are missing this information.
# Each result is devided into 24 Livermore Loops (aka kernels). And into Optimized and Non-Optimized results.
# 
# Your task: (to Gemini AI)
# 
# Develop a python script, that reads all the result files (in *.txt format), analyzes CPU architectures, every single one of them, that have "Clockrate and TDP" values, strong points and weak points of each.
# Analysis goals are as follows:
# 
# 1. Python script must read all *.txt files from current directory, and skip files with missing "Clockrate and TDP" values, and output HTML files into the very same current directory, where the python script is run.
# 2. Normalize every CPU, single-core performance down to 1 GHz. so a CPU with a score with single-core score of 7000 running at 3500 MHz should have a normalized score of 2000/GHz. Do this both for overall single-core score, as well as per kernel (livermore loop).
# 3. Normalize every CPU, multi-core performance down to TDP levels, for every watt of TDP. For example: for a CPU X, that shows a 2000 multi-core score, and consumes 20 watts, it produces 100 multi-core score per watt. Do this both for overall score, as well as per kernel. Do this both for overall multi-core score, as well as per kernel (livermore loop).
# 4. Normalizing per 1 GHz for multi-core benchmark makes no sense, because modern CPUs have floating clock-rates, not fixed clocks, and they will throttle after a while, both desktops and laptops. TDP for a single-core is meanningless, because a single-core will not utilize a whole chip's TDP anyway, except for very ancient chips, that were designed as single-core.
# 5. ancient CPUs do have a fixed clock rates. Like Pentium 1/2/3/4/M. But they are typically single-core, and 32-bit. So easier to analyze those.
# 6. the script must be self contained, and not use any extra python modules from pip. Only the standard python library, that is included with python 3.
# 7. the script must work on both Windows, Mac and Linux systems.
# 8. the script must generate four HTML files as outputs, with 24 kernels each + overall score. 
# 8.1. HTML with single-core optimized results
# 8.2. HTML with multi-core optimized results
# 8.3. HTML with single-core non-optimized results
# 8.4. HTML with multi-core non-optimized results
# 9. The HTML files should build a table (spreadsheet), with color-coded columns.
# Rows must be the CPU name. Columns must be the normalized results (single-core / 1 GHz) and (Multi-core / TDP).
# The color codes should be relative and go from red (worst case) to yellow and then to green (best case), one color per cell.
# 10. The columns must be separated into 2 parts via a some kind of vertical line or an empty cell :
# Part A: overall score
# Part B: per kernel score (1...24).
# We will end up with 25 columns (overall score +24 kernels)  and about 200 rows (CPU models). Each cell must include both a human readable normalized value, black text font + background color code relative to other models, ranging from red to green. 
# 11. We are building a CPU architecture analysis tool, that can quickly show off strong and weak spots of every single CPU architecture that was ever benchmarked on the planet.
# 12. Color coded HTML is necessary for humans to be able to read it quickly. Humans are hardware coded to recognize color codes in a single cycle, split second, instead of reading 25 x 200 = 5000 values (or 5000 cycles or 5000 seconds).
# 13. HTML format must be very simple, static HTML4 style, so it is readable offline by ancient browsers, like Internet Explorer 6 and FireFox 1.0.
# 14. Use natural search where possible (so that 933 MHz --> 1000 MHz, in that order).
# 15. Extended: generate both "raw" and "normalized" versions.
#
#


import os
import glob
import re

def natural_sort_key(text):
    """
    Splits a string into text and integer components for human-readable sorting.
    Example: "Pentium III 1000" -> ["pentium iii ", 1000, ""]
    """
    return [int(chunk) if chunk.isdigit() else chunk.lower() for chunk in re.split(r'(\d+)', text)]

def get_color_hex(value, min_val, max_val):
    """Generates an HTML color code from Red (min) to Yellow to Green (max)."""
    if max_val <= min_val:
        return "#FFFF00" # Default to yellow if no variance
    
    # Calculate percentage (0.0 to 1.0)
    p = (value - min_val) / (max_val - min_val)
    
    if p < 0.5:
        # Red to Yellow
        r = 255
        g = int(255 * (2 * p))
    else:
        # Yellow to Green
        g = 255
        r = int(255 * (1 - 2 * (p - 0.5)))
        
    return f"#{r:02X}{g:02X}00"

def parse_benchmark_files(directory):
    cpus = {}
    file_pattern = os.path.join(directory, "*.txt")
    
    # Grab all files and sort them naturally into RAM before processing
    all_files = glob.glob(file_pattern)
    sorted_files = sorted(all_files, key=natural_sort_key)
    
    for filepath in sorted_files:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
            
        # Extract critical metadata
        clock_match = re.search(r'(?i)CLOCKRATE_MAX\s*-\s*([\d\.]+)', content)
        tdp_match = re.search(r'(?i)TDP_MAX\s*-\s*([\d\.]+)', content)
        
        if not clock_match or not tdp_match:
            print(f"Skipping {os.path.basename(filepath)}: Missing Clockrate or TDP.")
            continue
            
        clock_mhz = float(clock_match.group(1))
        tdp = float(tdp_match.group(1))
        clock_ghz = clock_mhz / 1000.0
        
        if clock_ghz <= 0 or tdp <= 0:
            print(f"Skipping {os.path.basename(filepath)}: Invalid Clockrate or TDP values.")
            continue

        # Extract CPU Name from internal text
        cpu_match = re.search(r'(?i)CPU name\s*(?:\r?\n)?\s*-\s*([^\n]+)', content)
        cpu_name = cpu_match.group(1).strip() if cpu_match else os.path.basename(filepath)
        
        # Split content into Non-Optimized and Optimized sections
        parts = re.split(r'(?i)Optimized score\s*-', content)
        if len(parts) < 2:
            print(f"Skipping {os.path.basename(filepath)}: Could not separate optimized/non-optimized sections.")
            continue
            
        non_opt_text = parts[0]
        opt_text = parts[1]
        
        def extract_scores(text_block):
            # We now separate scores into raw and norm (normalized)
            scores = {
                'single': {'raw': {}, 'norm': {}}, 
                'multi': {'raw': {}, 'norm': {}}
            }
            
            # Extract Overall scores
            single_ov = re.search(r'(?i)Single core\s*-\s*([\d\.]+)', text_block)
            multi_ov = re.search(r'(?i)Multi core\s*-\s*([\d\.]+)', text_block)
            
            s_val = float(single_ov.group(1)) if single_ov else 0.0
            m_val = float(multi_ov.group(1)) if multi_ov else 0.0
            
            scores['single']['raw'][0] = s_val
            scores['single']['norm'][0] = s_val / clock_ghz
            
            scores['multi']['raw'][0] = m_val
            scores['multi']['norm'][0] = m_val / tdp
            
            # Extract 24 Kernel loops
            for i in range(1, 25):
                loop_match = re.search(rf'\|\s*{i}\s*\|\s*([\d\.]+)\s*\|\s*([\d\.]+)', text_block)
                if loop_match:
                    s_loop = float(loop_match.group(1))
                    m_loop = float(loop_match.group(2))
                    
                    scores['single']['raw'][i] = s_loop
                    scores['single']['norm'][i] = s_loop / clock_ghz
                    
                    scores['multi']['raw'][i] = m_loop
                    scores['multi']['norm'][i] = m_loop / tdp
                else:
                    scores['single']['raw'][i] = 0.0
                    scores['single']['norm'][i] = 0.0
                    scores['multi']['raw'][i] = 0.0
                    scores['multi']['norm'][i] = 0.0
                    
            return scores
            
        if cpu_name in cpus:
            print(f"Warning: Overwriting duplicate internal CPU name '{cpu_name}' found in {os.path.basename(filepath)}")

        cpus[cpu_name] = {
            'non_opt': extract_scores(non_opt_text),
            'opt': extract_scores(opt_text)
        }
        
    return cpus

def generate_html(filename, title, data_dict, metric_label):
    if not data_dict:
        return
        
    # Calculate min and max for color scaling (0 is overall, 1-24 are loops)
    min_max = {}
    for col in range(25):
        valid_vals = [cpu_data[col] for cpu_data in data_dict.values() if cpu_data[col] > 0]
        if valid_vals:
            min_max[col] = (min(valid_vals), max(valid_vals))
        else:
            min_max[col] = (0.0, 0.0)

    html = [
        '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">',
        '<html><head>',
        f'<title>{title}</title>',
        '<style>',
        'body { font-family: Arial, sans-serif; background-color: #FFFFFF; color: #000000; margin: 20px; }',
        'table { border-collapse: collapse; width: 100%; font-size: 12px; }',
        'th, td { border: 1px solid #333333; padding: 4px; text-align: right; white-space: nowrap; }',
        'th { background-color: #DDDDDD; text-align: center; font-weight: bold; }',
        '.cpu-name { text-align: left; background-color: #EEEEEE; font-weight: bold; }',
        '.separator { background-color: #000000; width: 4px; padding: 0; }',
        '</style>',
        '</head><body>',
        f'<h2>{title}</h2>',
        f'<p><b>Metric:</b> {metric_label}</p>',
        '<table>',
        '<tr><th>CPU Model</th><th>Overall</th><th class="separator"></th>'
    ]
    
    # Header row for loops
    for i in range(1, 25):
        html.append(f'<th>L{i}</th>')
    html.append('</tr>')
    
    # Sort CPUs alphabetically
    #for cpu_name in sorted(data_dict.keys()):
    # Sort CPUs using natural human sorting (e.g., 933 MHz before 1000 MHz)
    for cpu_name in sorted(data_dict.keys(), key=natural_sort_key):
        html.append(f'<tr><td class="cpu-name">{cpu_name}</td>')
        
        for col in range(25):
            val = data_dict[cpu_name].get(col, 0.0)
            if col == 1:
                # Insert the visual separator before loop 1
                html.append('<td class="separator"></td>')
                
            if val > 0:
                bg_color = get_color_hex(val, min_max[col][0], min_max[col][1])
                # Print rounded to 2 decimal places
                html.append(f'<td bgcolor="{bg_color}">{val:.2f}</td>')
            else:
                html.append('<td bgcolor="#AAAAAA">N/A</td>')
                
        html.append('</tr>')
        
    html.append('</table></body></html>')
    
    with open(filename, 'w', encoding='utf-8') as f:
        f.write('\n'.join(html))
    print(f"Generated: {filename}")

if __name__ == "__main__":
    current_dir = os.getcwd()
    print(f"Scanning directory: {current_dir}")
    
    cpu_data = parse_benchmark_files(current_dir)
    print(f"Successfully parsed {len(cpu_data)} valid CPU reports.\n")
    
    # Reorganize data into the 8 required permutations
    # Single Core / Optimized
    sc_opt_norm = {name: data['opt']['single']['norm'] for name, data in cpu_data.items()}
    sc_opt_raw = {name: data['opt']['single']['raw'] for name, data in cpu_data.items()}
    # Single Core / Non-Optimized
    sc_non_opt_norm = {name: data['non_opt']['single']['norm'] for name, data in cpu_data.items()}
    sc_non_opt_raw = {name: data['non_opt']['single']['raw'] for name, data in cpu_data.items()}
    
    # Multi Core / Optimized
    mc_opt_norm = {name: data['opt']['multi']['norm'] for name, data in cpu_data.items()}
    mc_opt_raw = {name: data['opt']['multi']['raw'] for name, data in cpu_data.items()}
    # Multi Core / Non-Optimized
    mc_non_opt_norm = {name: data['non_opt']['multi']['norm'] for name, data in cpu_data.items()}
    mc_non_opt_raw = {name: data['non_opt']['multi']['raw'] for name, data in cpu_data.items()}
    
    # Generate the 8 HTML tables
    generate_html("1_SingleCore_Opt_Normalized.html", "Single-Core Optimized (Normalized)", sc_opt_norm, "Score per 1 GHz")
    generate_html("2_SingleCore_Opt_Raw.html", "Single-Core Optimized (Raw Data)", sc_opt_raw, "Absolute Score")
    
    generate_html("3_SingleCore_NonOpt_Normalized.html", "Single-Core Non-Optimized (Normalized)", sc_non_opt_norm, "Score per 1 GHz")
    generate_html("4_SingleCore_NonOpt_Raw.html", "Single-Core Non-Optimized (Raw Data)", sc_non_opt_raw, "Absolute Score")
    
    generate_html("5_MultiCore_Opt_Normalized.html", "Multi-Core Optimized (Normalized)", mc_opt_norm, "Score per 1 Watt (TDP)")
    generate_html("6_MultiCore_Opt_Raw.html", "Multi-Core Optimized (Raw Data)", mc_opt_raw, "Absolute Score")
    
    generate_html("7_MultiCore_NonOpt_Normalized.html", "Multi-Core Non-Optimized (Normalized)", mc_non_opt_norm, "Score per 1 Watt (TDP)")
    generate_html("8_MultiCore_NonOpt_Raw.html", "Multi-Core Non-Optimized (Raw Data)", mc_non_opt_raw, "Absolute Score")
    
    print("\nAll 8 HTML reports generated successfully.")