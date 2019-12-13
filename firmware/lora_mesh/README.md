# Supporting Libraries

### Grove_LoRa_433MHz_and_915MHz_RF
- LoRa radio library from Seeed Studio to interface with the Grove LoRa radio. It uses the RadioHead library.

- Information can be found in the RadioHead documentation [here](https://www.airspayce.com/mikem/arduino/RadioHead/index.html).

### Chunk
- Class to represent a large chunk of data called Chunk that is made up of smaller chunks called Segment.

- Each Chunk is represented with a unique identifier in the network.

### Transport
- Class to handle the transmission and reception of Chunks.

- Each Chunk is fragmented into smaller Segments and each Segment is transmitted one by one through the network to reach the destination address.

- The receiving application collects individual Segments and reassemble them back into it's original Chunk using the unique identifier.

### circular_buffer
- FIFO data structure to store Chunks using a circular buffer.

### serial_reader
- Helper module to read data from Serial port and turn them into Chunks.

### crc
- Helper module to handle 16 bits CRC checksum calculations and checks.

