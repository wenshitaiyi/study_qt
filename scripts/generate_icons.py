import os
import zlib
import struct
import math

def create_png(width, height, rgba_data):
    """Generate a standard PNG file bytes from RGBA pixel array using pure python standard library."""
    # PNG signature
    png = b'\x89PNG\r\n\x1a\n'
    
    # IHDR chunk
    ihdr_data = struct.pack('>IIBBBBB', width, height, 8, 6, 0, 0, 0)
    ihdr_crc = zlib.crc32(b'IHDR' + ihdr_data) & 0xffffffff
    png += struct.pack('>I', len(ihdr_data)) + b'IHDR' + ihdr_data + struct.pack('>I', ihdr_crc)
    
    # Raw image scanlines
    raw_data = bytearray()
    for y in range(height):
        raw_data.append(0) # Filter type 0 (None)
        for x in range(width):
            idx = (y * width + x) * 4
            raw_data.extend(rgba_data[idx:idx+4])
            
    # IDAT chunk
    compressed = zlib.compress(bytes(raw_data), 9)
    idat_crc = zlib.crc32(b'IDAT' + compressed) & 0xffffffff
    png += struct.pack('>I', len(compressed)) + b'IDAT' + compressed + struct.pack('>I', idat_crc)
    
    # IEND chunk
    iend_crc = zlib.crc32(b'IEND') & 0xffffffff
    png += struct.pack('>I', 0) + b'IEND' + struct.pack('>I', iend_crc)
    
    return png

def generate_game_icon(index, width=64, height=64):
    """Generate a 64x64 game-style item icon with rounded frame, gradient, and category symbol."""
    pixels = bytearray(width * height * 4)
    
    # 5 Color Themes (Common: Gray/Green, Rare: Blue, Epic: Purple, Legendary: Gold/Orange, Immortal: Red)
    themes = [
        # (bg_top, bg_bot, border, inner_symbol_color)
        ((46, 204, 113), (39, 174, 96), (230, 240, 230), (255, 255, 255)),   # Green (Common)
        ((52, 152, 219), (41, 128, 185), (200, 230, 255), (255, 255, 255)),  # Blue (Rare)
        ((155, 89, 182), (142, 68, 173), (240, 220, 255), (255, 255, 255)),  # Purple (Epic)
        ((241, 196, 15), (230, 126, 34), (255, 245, 180), (255, 255, 255)),  # Gold/Orange (Legendary)
        ((231, 76, 60),  (192, 57, 43),  (255, 220, 220), (255, 255, 255)),  # Red (Immortal)
    ]
    
    theme = themes[index % len(themes)]
    bg_top, bg_bot, border_col, symbol_col = theme
    
    cx, cy = width / 2.0, height / 2.0
    r = 28.0
    
    for y in range(height):
        for x in range(width):
            idx = (y * width + x) * 4
            
            # Distance from rounded box
            # Box bounds: [4, 4] to [60, 60], corner radius = 8
            dx = max(0, abs(x - cx) - (28 - 8))
            dy = max(0, abs(y - cy) - (28 - 8))
            dist = math.sqrt(dx*dx + dy*dy)
            
            if dist > 8.0:
                # Transparent outside rounded corner
                pixels[idx:idx+4] = [0, 0, 0, 0]
                continue
                
            # Border check (outer 2px)
            is_border = (dist >= 6.5) or (x <= 5 or x >= 58 or y <= 5 or y >= 58)
            
            if is_border:
                pixels[idx:idx+4] = [border_col[0], border_col[1], border_col[2], 255]
            else:
                # Gradient background
                t = y / float(height)
                r_c = int(bg_top[0] * (1 - t) + bg_bot[0] * t)
                g_c = int(bg_top[1] * (1 - t) + bg_bot[1] * t)
                b_c = int(bg_top[2] * (1 - t) + bg_bot[2] * t)
                
                # Draw geometric inner badge/symbol based on (index // 5) % 10
                shape_type = (index // len(themes)) % 10
                is_shape = False
                
                # Center coordinates offset
                ox, oy = x - cx, y - cy
                
                if shape_type == 0: # Sword / Diamond
                    is_shape = (abs(ox) + abs(oy)) <= 14 or (abs(ox) <= 3 and abs(oy) <= 20) or (abs(oy) <= 3 and abs(ox) <= 12 and oy > 8)
                elif shape_type == 1: # Shield
                    is_shape = (abs(ox) <= 14) and (oy <= 8 and oy >= -14) and (oy <= 14 - abs(ox)*1.2)
                elif shape_type == 2: # Potion / Flask
                    is_shape = (ox*ox + oy*oy <= 14*14 and oy >= -4) or (abs(ox) <= 5 and oy >= -16 and oy <= -4)
                elif shape_type == 3: # Crystal / Hexagon
                    is_shape = (abs(ox) <= 14 and abs(oy) <= 16 and (abs(ox)*1.5 + abs(oy)) <= 24)
                elif shape_type == 4: # Ring / Circle
                    d_c = math.sqrt(ox*ox + oy*oy)
                    is_shape = (d_c >= 8 and d_c <= 15)
                elif shape_type == 5: # Helmet / Arc
                    is_shape = (ox*ox + oy*oy <= 15*15 and oy <= 4) or (abs(ox) >= 8 and abs(ox) <= 14 and oy >= 0 and oy <= 12)
                elif shape_type == 6: # Axe / Cross
                    is_shape = (abs(ox) <= 3 and abs(oy) <= 18) or (abs(oy + 6) <= 10 and abs(ox) <= 14)
                elif shape_type == 7: # Bow / Crescent
                    d1 = math.sqrt((ox+6)*(ox+6) + oy*oy)
                    d2 = math.sqrt((ox+12)*(ox+12) + oy*oy)
                    is_shape = (d1 <= 18 and d2 >= 18) or (abs(oy) <= 2 and ox >= -12 and ox <= 14)
                elif shape_type == 8: # Scroll / Book
                    is_shape = (abs(ox) <= 14 and abs(oy) <= 12) and not (abs(ox) <= 2 and abs(oy) <= 10)
                else: # Star / Crown
                    is_shape = (abs(ox) <= 16 and oy >= -2 and oy <= 12) or (abs(ox) <= 4 and oy >= -14 and oy <= 12) or (abs(ox - 12) <= 3 and oy >= -10 and oy <= 12) or (abs(ox + 12) <= 3 and oy >= -10 and oy <= 12)
                
                if is_shape:
                    pixels[idx:idx+4] = [symbol_col[0], symbol_col[1], symbol_col[2], 255]
                else:
                    pixels[idx:idx+4] = [r_c, g_c, b_c, 255]
                    
    return create_png(width, height, pixels)

def main():
    target_dirs = [
        os.path.join("data", "icons"),
        os.path.join("app", "data", "icons"),
        os.path.join("app", "debug", "data", "icons"),
        os.path.join("app", "release", "data", "icons")
    ]
    
    for d in target_dirs:
        os.makedirs(d, exist_ok=True)
        
    print(f"Generating 50 64x64 PNG game icons...")
    for i in range(1, 51):
        png_data = generate_game_icon(i - 1)
        filename = f"icon_{i:02d}.png"
        for d in target_dirs:
            path = os.path.join(d, filename)
            with open(path, "wb") as f:
                f.write(png_data)
                
    print(f"Successfully generated 50 icons in:")
    for d in target_dirs:
        print(f"  -> {d}")

if __name__ == "__main__":
    main()
