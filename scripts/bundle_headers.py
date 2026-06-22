#!/usr/bin/env python3
"""
Bundle script: for each public header in include/libcpprime/*.hpp,
expand all local #include "..." directives (both .hpp and .txt)
to produce a self-contained file in bundled/.
"""

import os
import re
import sys

LOCAL_INCLUDE_RE = re.compile(r'#\s*include\s+"([^"]+)"')

CPPR_LICENSE = re.compile(
    r'/\*\*\n'
    r' \*\n'
    r' \* libcpprime https://github\.com/sortA0329/libcpprime\n'
    r' \*\n'
    r' \* Copyright \(c\) 2026 sortA\n'
    r' \* SPDX-License-Identifier: MIT\n'
    r' \*\n'
    r' \*\*/\n?'
)


def strip_cppr_license(text):
    return CPPR_LICENSE.sub('', text, count=1)


def bundle_file(filepath, processed, strip_license=False):
    abs_path = os.path.normpath(filepath)
    base_dir = os.path.dirname(abs_path)

    with open(abs_path, 'r') as f:
        content = f.read()

    if strip_license:
        content = strip_cppr_license(content)

    def replace_include(match):
        include_path = match.group(1)
        resolved = os.path.normpath(os.path.join(base_dir, include_path))

        if not os.path.exists(resolved):
            print(f"Warning: {include_path} not found (resolved to {resolved})", file=sys.stderr)
            return match.group(0)

        resolved = os.path.normpath(resolved)

        if resolved in processed:
            return ''

        processed.add(resolved)
        return bundle_file(resolved, processed, strip_license=True)

    return LOCAL_INCLUDE_RE.sub(replace_include, content)


def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    repo_root = os.path.normpath(os.path.join(script_dir, '..'))
    include_dir = os.path.join(repo_root, 'include', 'libcpprime')
    output_dir = os.path.join(repo_root, 'bundled')

    os.makedirs(output_dir, exist_ok=True)

    hpp_files = sorted([
        f for f in os.listdir(include_dir)
        if f.endswith('.hpp') and os.path.isfile(os.path.join(include_dir, f))
    ])

    for hpp_file in hpp_files:
        filepath = os.path.join(include_dir, hpp_file)
        print(f"Bundling {hpp_file}...")

        processed = set()
        processed.add(os.path.normpath(filepath))
        bundled = bundle_file(filepath, processed)

        output_path = os.path.join(output_dir, hpp_file)
        with open(output_path, 'w') as f:
            f.write(bundled)

        print(f"  -> {output_path}")

    print("Done.")


if __name__ == '__main__':
    main()
