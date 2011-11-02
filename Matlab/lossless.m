% attempt to do some entropy encoding
% JPEG does this:
% - arranges image components in a zigzag order and then employ run length
% encoding
% - use huffman encoding on what is left (supports arithmatic encoding but
% rarely used)
% Maybe look at LZW algo too
