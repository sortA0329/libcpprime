import subprocess


def create_bundled(filename: str) -> None:
    try:
        result = subprocess.run(
            [
                "clang++",
                "-nostdinc",
                "-fdirectives-only",
                "-E",
                "-P",
                "-CC",
                f"./libcpprime/{filename}.hpp",
            ],
            capture_output=True,
            text=True,
        )
        with open(f"./bundled/{filename}.hpp", "w", encoding="utf-8") as f:
            f.write(result.stdout)
    except subprocess.CalledProcessError:
        pass


if __name__ == "__main__":
    create_bundled("IsPrime")
    create_bundled("IsPrimeNoTable")
