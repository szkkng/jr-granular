{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

  outputs =
    { self, nixpkgs }:
    let
      allSystems = [
        "x86_64-darwin"
        "aarch64-darwin"
        "x86_64-linux"
        "aarch64-linux"
      ];

      forAllSystems =
        f: nixpkgs.lib.genAttrs allSystems (system: f { pkgs = import nixpkgs { inherit system; }; });
    in
    {
      devShells = forAllSystems (
        { pkgs }:
        {
          default = pkgs.mkShell.override { stdenv = pkgs.llvmPackages.stdenv; } {
            packages =
              with pkgs;
              [
                cmake
                ninja
                sccache
                llvmPackages.clang-tools
                gersemi
              ]
              ++ lib.optionals stdenv.isLinux [
                pkg-config
                llvmPackages.bintools

                # https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md#packages
                # juce_audio_devices
                alsa-lib
                libjack2

                # juce_audio_processors
                ladspa-sdk

                # juce_core
                curl

                # juce_graphics
                fontconfig
                freetype

                # juce_gui_basics
                xorg.libX11
                xorg.libXcomposite
                xorg.libXcursor
                xorg.libXext
                xorg.libXinerama
                xorg.libXrandr
                xorg.libXrender

                # juce_gui_extra
                webkitgtk_4_1

                # others
                libuuid
                libxkbcommon
                libthai
                libdatrie
                libepoxy
                libselinux
                libsepol
                libsysprof-capture
                xorg.libXdmcp
                xorg.libXtst
                lerc
                pcre2
                sqlite
              ];

            hardeningDisable = [ "fortify" ];

            # Prevent JUCE_XCRUN from picking up Nix's xcrun,
            # which can cause 'codesign' to fail on macOS (error: tool 'codesign' not found).
            # Ensures that the system xcrun (from Xcode) is used instead.
            shellHook = pkgs.lib.optionalString pkgs.stdenv.isDarwin ''
              export PATH=$(echo "$PATH" | sed "s|${pkgs.xcbuild.xcrun}/bin:||g")
              unset DEVELOPER_DIR
            '';

            NIX_LDFLAGS = toString (
              pkgs.lib.optionals pkgs.stdenv.isLinux [
                "-lX11"
                "-lXext"
                "-lXcursor"
                "-lXinerama"
                "-lXrandr"
              ]
            );
          };
        }
      );
    };
}
