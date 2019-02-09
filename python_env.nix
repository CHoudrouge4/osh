with import <nixpkgs> {};
stdenv.mkDerivation rec {
  name = "env";
  env = buildEnv { name = name; paths = buildInputs; };
  buildInputs = [
    python3
    python37Packages.matplotlib
    python37Packages.jupyter
    python37Packages.ipython
    python37Packages.pandas
  ];
}

