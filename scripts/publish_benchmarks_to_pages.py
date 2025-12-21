#!/usr/bin/env python3

from __future__ import annotations

import argparse
import json
import shutil
import zipfile
from pathlib import Path


def _safe_rmtree(path: Path) -> None:
    if path.exists():
        shutil.rmtree(path)


def _copy_tree(src: Path, dst: Path) -> None:
    dst.mkdir(parents=True, exist_ok=True)
    for item in src.iterdir():
        target = dst / item.name
        if item.is_dir():
            shutil.copytree(item, target, dirs_exist_ok=True)
        else:
            shutil.copy2(item, target)


def _extract_zip(zip_path: Path, dst: Path) -> None:
    dst.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(zip_path) as z:
        z.extractall(dst)


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Publish benchmark artifacts to GitHub Pages (fixed latest path)."
    )
    parser.add_argument("--artifacts-dir", required=True)
    parser.add_argument("--site-dir", required=True)
    parser.add_argument("--repo", required=True, help="owner/repo")
    parser.add_argument("--run-id", required=True)
    args = parser.parse_args()

    artifacts_dir = Path(args.artifacts_dir)
    site_dir = Path(args.site_dir)

    repo = args.repo
    run_id = int(args.run_id)
    run_url = (
        f"https://github.com/{repo}/actions/runs/{run_id}" if repo and run_id else None
    )

    # Fixed, always-latest path.
    benchmarks_dir = site_dir / "benchmarks"
    latest_dir = benchmarks_dir / "latest"

    # Start fresh to avoid stale files from older runs.
    latest_dir.mkdir(parents=True, exist_ok=True)
    _safe_rmtree(latest_dir)
    latest_dir.mkdir(parents=True, exist_ok=True)

    published_artifacts: list[str] = []

    if artifacts_dir.exists():
        for entry in sorted(artifacts_dir.iterdir(), key=lambda p: p.name):
            name = entry.name

            if entry.is_dir():
                # actions/download-artifact typically extracts into a directory per artifact.
                if not name.startswith("benchmark-"):
                    continue
                _copy_tree(entry, latest_dir / name)
                published_artifacts.append(name)
                continue

            if entry.is_file() and entry.suffix.lower() == ".zip":
                base = entry.stem
                if not base.startswith("benchmark-"):
                    continue
                _extract_zip(entry, latest_dir / base)
                published_artifacts.append(base)
                continue

    # No Jekyll processing (serve files as-is)
    site_dir.mkdir(parents=True, exist_ok=True)
    (site_dir / ".nojekyll").write_text("", encoding="utf-8")

    benchmarks_dir.mkdir(parents=True, exist_ok=True)

    latest_json = {
        "runId": run_id,
        "runUrl": run_url,
        "artifacts": published_artifacts,
        "baseUrl": "/benchmarks/latest",
    }

    (benchmarks_dir / "latest.json").write_text(
        json.dumps(latest_json, ensure_ascii=False, indent=2) + "\n", encoding="utf-8"
    )

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
