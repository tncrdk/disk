{
  description = "Disk benchmarking";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        system = "x86_64-linux";
        pkgs = nixpkgs.legacyPackages.${system};
      in {
        packages.default = pkgs.stdenv.mkDerivation rec {
          name = "disk_benchmarks";
          src = self;
          version = "git";
          buildInputs = with pkgs; [ cmake gcc ];
          cmakeFlags = [
            "-DCMAKE_BUILD_TYPE=Release"
          ];
          configurePhase = "cmake -S $src -B build -DCMAKE_INSTALL_PREFIX=$out";
          buildPhase = "cmake --build build";
          # installPhase = "mkdir -p $out/bin; install -t $out/bin hello";
          installPhase = "cmake --install build";
        };
      });
}
