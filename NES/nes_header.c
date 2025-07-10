#include <stdio.h>
#include <stdint.h>

void dump_nes_header(uint8_t *rom) {
    printf("Magic number: %.4s\n", rom);
    uint8_t prg_rom_size = rom[4];
    printf("PRG ROM size: %d x 16 KB\n", prg_rom_size);
    uint8_t chr_rom_size = rom[5];
    printf("CHR ROM size: %d x 8 KB\n", chr_rom_size);
    uint8_t flags6 = rom[6];
    printf("Flags 6: 0x%02X\n", flags6);
    printf("  Mapper: %d\n", (flags6 >> 4) | (rom[7] & 0xF0));
    printf("  Mirroring: %s\n", (flags6 & 0x01) ? "Vertical" : "Horizontal");
    uint8_t flags7 = rom[7];
    printf("Flags 7: 0x%02X\n", flags7);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <rom_file>\n", argv[0]);
        return 1;
    }

    FILE *rom_file = fopen(argv[1], "rb");
    if (rom_file == NULL) {
        printf("Failed to open ROM file '%s'\n", argv[1]);
        return 1;
    }

    uint8_t rom_header[16];
    fread(rom_header, 1, 16, rom_file);

    if (rom_header[0] != 'N' || rom_header[1] != 'E' || rom_header[2] != 'S' || rom_header[3] != 0x1A) {
        printf("Invalid NES ROM file\n");
        return 1;
    }

    dump_nes_header(rom_header);

    fclose(rom_file);
    return 0;
}