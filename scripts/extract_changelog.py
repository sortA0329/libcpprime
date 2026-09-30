"""Extract the changelog for a given version from README.md.

The README contains a "Releases" section.
This script extracts the bullet points of the requested version so that
they can be used as the body of a GitHub Release.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

VERSION_HEADER_RE = re.compile(r"^\s*-\s+\S+\s+v\s*(\d+\S*)\s*$")
HEADING_RE = re.compile(r"^#{1,6}\s")


def normalize_version(version: str) -> str:
    version = version.strip()
    if version[:1] in ("v", "V"):
        version = version[1:]
    return version


def find_available_versions(lines: list[str]) -> list[str]:
    versions = []
    for line in lines:
        match = VERSION_HEADER_RE.match(line)
        if match:
            versions.append(normalize_version(match.group(1)))
    return versions


def find_latest_version(readme_text: str) -> str:
    for line in readme_text.splitlines():
        match = VERSION_HEADER_RE.match(line)
        if match:
            return normalize_version(match.group(1))
    raise ValueError("No version found in README.md.")


def extract_changelog(readme_text: str, version: str) -> str:
    lines = readme_text.splitlines()

    start = None
    for i, line in enumerate(lines):
        match = VERSION_HEADER_RE.match(line)
        if match and normalize_version(match.group(1)) == version:
            start = i + 1
            break

    if start is None:
        available = ", ".join(find_available_versions(lines)) or "(none)"
        raise ValueError(
            f"Version {version!r} not found in README.md. "
            f"Available versions: {available}"
        )

    body: list[str] = []
    for line in lines[start:]:
        if HEADING_RE.match(line):
            break
        if VERSION_HEADER_RE.match(line):
            break
        body.append(line.rstrip())

    while body and not body[0].strip():
        body.pop(0)
    while body and not body[-1].strip():
        body.pop()

    if not body or not any(line.strip() for line in body):
        raise ValueError(f"No changelog entries found for version {version!r}.")

    # Dedent by the common leading indentation so that nested bullets
    # become top-level bullets in the release body.
    indent = min(len(line) - len(line.lstrip(" ")) for line in body if line.strip())
    dedented = [line[indent:] if len(line) >= indent else line for line in body]
    return "\n".join(dedented) + "\n"


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Extract the changelog for a given version from README.md."
    )
    parser.add_argument(
        "--version",
        default=None,
        help="Version to extract (e.g. 1.4.0 or v1.4.0).",
    )
    parser.add_argument(
        "--latest",
        action="store_true",
        help="Print the latest version from README.md instead of extracting.",
    )
    parser.add_argument(
        "--readme",
        default="README.md",
        help="Path to README.md (default: README.md).",
    )
    parser.add_argument(
        "--output",
        default=None,
        help="Output file. Defaults to stdout.",
    )
    args = parser.parse_args()

    readme_path = Path(args.readme)
    if not readme_path.is_file():
        print(f"Error: README not found: {readme_path}", file=sys.stderr)
        return 1

    readme_text = readme_path.read_text(encoding="utf-8")

    if args.latest:
        try:
            output_text = find_latest_version(readme_text) + "\n"
        except ValueError as exc:
            print(f"Error: {exc}", file=sys.stderr)
            return 1
    else:
        if args.version is None:
            parser.error("--version is required unless --latest is specified.")
        version = normalize_version(args.version)

        try:
            output_text = extract_changelog(readme_text, version)
        except ValueError as exc:
            print(f"Error: {exc}", file=sys.stderr)
            return 1

    if args.output:
        Path(args.output).write_text(output_text, encoding="utf-8")
    else:
        sys.stdout.write(output_text)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
