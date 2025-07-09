#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define ROM_HEADER_SIZE 0x200 // Size of the ROM header

void dump_rom_header(uint8_t *rom) {
    printf("Console Name: %.16s\n", rom + 0x100);
    printf("Copyright Information: %.16s\n", rom + 0x110);
    printf("Game Title: %.48s\n", rom + 0x120);
    printf("Game Serial Number: %.14s\n", rom + 0x180);
    uint16_t checksum = (rom[0x18E] << 8) | rom[0x18F];
    printf("Checksum: 0x%04X\n", checksum);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <rom_file>\n", argv[0]);
        return 1;
    }

    FILE *rom_file = fopen(argv[1], "rb+");
    if (!rom_file) {
        printf("Failed to open ROM file\n");
        return 1;
    }

    uint8_t rom_header[ROM_HEADER_SIZE];
    fread(rom_header, 1, ROM_HEADER_SIZE, rom_file);

    dump_rom_header(rom_header);

    fclose(rom_file);
    return 0;
}