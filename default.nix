{ pkgs ? import <nixpkgs> { } } :
pkgs.mkShell {
    buildInputs = with pkgs; [
        vulkan-headers
        vulkan-tools
        vulkan-loader
        glfw
    ];

    LD_LIBRARY_PATH = with pkgs; ''
        ${musl.outPath}/lib:${glfw.outPath}/lib:${vulkan-loader.outPath}/lib
    '';

    shellHook = with pkgs; ''
        gcc -no-pie -lglfw -lvulkan \
        -I${glfw.outPath}/include/GLFW \
        -I${vulkan-headers.outPath}/include/vulkan \
        -I${vulkan-headers.outPath}/include/vk_video \
        -o main main.c
    '';
}