#include "stdint.h"
struct BootSector
{
    //uint8_t BootJumpInstruction[3];
    //uint8_t OemIdentifier[8];
    uint16_t BytesPerSector;
    //uint8_t SectorsPerCluster;
    uint16_t ReservedSectors;
    uint8_t FatCount;
    uint16_t DirEntryCount;
    //uint16_t TotalSectors;
    //uint8_t MediaDescriptorType;
    uint16_t SectorsPerFat;
    //uint16_t SectorsPerTrack;
    //uint16_t Heads;
    //uint32_t HiddenSectors;
    //uint32_t LargeSectorCount;

    // extended boot record
    //uint8_t DriveNumber;
    //uint8_t _Reserved;
    //uint8_t Signature;
    //uint32_t VolumeId;          // serial number, value doesn't matter
    //uint8_t VolumeLabel[11];    //0 11 bytes, padded with spaces
    //uint8_t SystemId[8];

    // ... we don't care about code ...

};
struct DirectoryEntry
{
    uint8_t Name[11];
    uint8_t Attributes;
    uint8_t _Reserved;
    uint8_t CreatedTimeTenths;
    uint16_t CreatedTime;
    uint16_t CreatedDate;
    uint16_t AccessedDate;
    uint16_t FirstClusterHigh;
    uint16_t ModifiedTime;
    uint16_t ModifiedDate;
    uint16_t FirstClusterLow;
    uint32_t Size;
};

uint16_t load_addr_segment = 0x0050;
uint16_t load_addr_offset = 0x0000;
