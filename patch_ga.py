import re

def hex_to_bytes(h):
    h = h.replace(" ", "").replace("?", "00")
    return bytearray.fromhex(h)

def patch_code(in_file, out_file, byte_pairs):
    with open(in_file, "rb") as f:
        data = f.read()

    for orig, repl in byte_pairs:
        orig_bytes = hex_to_bytes(orig)
        orig_regex = re.compile(re.escape(orig_bytes).replace(b"\\\x00", b"."))

        matches = list(orig_regex.finditer(data))
        if matches:
            print(f"Found {len(matches)} matches for {orig}")
            repl_bytes = bytes.fromhex(repl.replace(' ', ''))
            if len(repl_bytes) < len(orig_bytes):
                repl_bytes += b'\x00' * (len(orig_bytes) - len(repl_bytes))
            data = orig_regex.sub(repl_bytes, data)
        else:
            print(f"No matches found for {orig}")

    with open(out_file, "wb") as f:
        f.write(data)

byte_sequences = [
    ("40 53 48 83 EC 20 8B D9 33 C9 E8 A1 73 49 FF 80 3D 33 35 40 02 00 75 12 8B 0D DA 39 94 01 E8 AD 41 25 FF C6 05 1F 35 40 02 01 48 8B 05 4F 07 43 02 45 33 C0 8B D3 48 8B 88 B8 00 00 00 48 8B 49 30 48 83 C4 20 5B E9 B5 7D 49 FF CC CC CC CC CC 48 83 EC 28 33 C9 E8 55 73 49 FF 80 3D E7 34 40 02 00 75 12 8B 0D 8E 39 94 01 E8 61 41 25 FF C6 05 D3 34 40 02 01 48 8B 05 03 07 43 02 33 D2 48 8B 88 B8 00 00 00 48 8B 49 30 48 83 C4 28 E9 FD 7D 49 FF CC CC CC CC CC CC CC CC CC CC CC CC CC 40 53 48 83 EC 20 8B D9 33 C9 E8 01 73 49 FF 80 3D 93 34 40 02 00 75 12 8B 0D 3A 39 94 01 E8 0D 41 25 FF C6 05 7F 34 40 02 01 48 8B 05 AF 06 43 02 45 33 C0 8B D3 48 8B 88 B8 00 00 00 48 8B 49 30 48 83 C4 20 5B E9 25 7E 49 FF CC CC CC CC CC 48 83 EC 28 33 C9 E8 B5 72 49 FF 80 3D 47 34 40 02 00 75 12 8B 0D EE 38 94 01 E8 C1 40 25 FF C6 05 33 34 40 02 01 48 8B 05 63 06 43 02 33 D2 48 8B 88 B8 00 00 00 48 8B 49 30 48 83 C4 28 E9 6D 7E 49 FF CC CC CC CC CC CC CC CC CC CC CC CC CC 48 83 EC", "48 B8 01 00 00 00 00 00 00 00 C3"),
    ("40 53 48 83 EC 20 8B D9 33 C9 E8 A1 6C 49 FF 80 3D 33 2E 40 02 00 75 12 8B 0D DA 32 94 01 E8 AD 3A 25 FF C6 05 1F 2E 40 02 01 48 8B 05 4F 00 43 02 45 33 C0 8B D3 48 8B 88 B8 00 00 00 48 8B 49 30 48 83 C4 20 5B E9 65 7F 49 FF CC CC CC CC CC 40 55 53", "B8 85 47 DE 63 C3"),
    ("48 83 EC 28 80 3D 30 26 8A 01 00 75 12 8B 0D 5D 5C D2 00 E8 28 B6 6D FE C6 05 1C 26 8A 01 01 48 8B 0D 82 92 92 01 F6 81 2F 01 00 00 02 74 0E 83 B9 E0 00 00 00 00 75 05 E8 63 07 66 FE 33 C9 E8 7C 01 00 00 84 C0 0F 85", "48 B8 01 00 00 00 00 00 00 00 C3")
]

input_file = "GameAssembly.dll"
output_file = "GameAssembly_patched.dll"

patch_code(input_file, output_file, byte_sequences)
