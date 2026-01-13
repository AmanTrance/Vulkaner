{ pkgs ? import <nixpkgs> { } } :
pkgs.mkShell {
    nativeBuildInputs = with pkgs; [
        glibc
    ];

    buildInputs = with pkgs; [
        vulkan-headers
        vulkan-tools
        vulkan-loader
        glfw
        shaderc
    ];

    LD_LIBRARY_PATH = with pkgs; ''
        ${glibc.outPath}/lib:${glfw.outPath}/lib:${vulkan-loader.outPath}/lib
    '';

    shellHook = with pkgs; ''
        gcc -pie -lglfw -lvulkan \
        -I${glfw.outPath}/include/GLFW \
        -I${vulkan-headers.outPath}/include/vulkan \
        -I${vulkan-headers.outPath}/include/vk_video \
        -o main *.c
    '';
}