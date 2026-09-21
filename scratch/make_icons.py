import os
import struct
import zlib
import math

def save_png(filename, width, height, pixels):
    raw_data = bytearray()
    for y in range(height):
        raw_data.append(0)
        for x in range(width):
            raw_data.extend(pixels[y][x])
    compressed = zlib.compress(bytes(raw_data))

    png = bytearray(b'\x89PNG\r\n\x1a\n')
    
    ihdr = struct.pack('>IIBBBBB', width, height, 8, 6, 0, 0, 0)
    ihdr_crc = zlib.crc32(b'IHDR' + ihdr)
    png.extend(struct.pack('>I', len(ihdr)) + b'IHDR' + ihdr + struct.pack('>I', ihdr_crc))
    
    idat_crc = zlib.crc32(b'IDAT' + compressed)
    png.extend(struct.pack('>I', len(compressed)) + b'IDAT' + compressed + struct.pack('>I', idat_crc))
    
    iend_crc = zlib.crc32(b'IEND')
    png.extend(struct.pack('>I', 0) + b'IEND' + struct.pack('>I', iend_crc))
    
    os.makedirs(os.path.dirname(filename), exist_ok=True)
    with open(filename, 'wb') as f:
        f.write(png)

def make_sword():
    W, H = 128, 128
    pix = [[(0, 0, 0, 0) for _ in range(W)] for _ in range(H)]
    for d in range(-45, 45):
        cx = 64 + d
        cy = 64 - d
        for r in range(-6, 7):
            x = cx + r
            y = cy + r
            if 0 <= x < W and 0 <= y < H:
                if d > -10:
                    edge = abs(r) == 6 or d == 44
                    mid = r == 0
                    if edge:
                        pix[y][x] = (60, 70, 85, 255)
                    elif mid:
                        pix[y][x] = (240, 248, 255, 255)
                    elif r > 0:
                        pix[y][x] = (200, 215, 235, 255)
                    else:
                        pix[y][x] = (140, 160, 185, 255)
                elif d >= -18:
                    pix[y][x] = (190, 160, 60, 255) if abs(r) <= 5 else (110, 90, 30, 255)
                elif d >= -38:
                    pix[y][x] = (120, 70, 30, 255) if abs(r) <= 3 else (70, 40, 15, 255)
                else:
                    pix[y][x] = (190, 160, 60, 255) if abs(r) <= 4 else (110, 90, 30, 255)
    for i in range(-18, 19):
        x = 54 + i
        y = 74 + i
        for w in range(-4, 5):
            px, py = x - w, y + w
            if 0 <= px < W and 0 <= py < H:
                edge = abs(w) == 4 or abs(i) == 18
                pix[py][px] = (100, 80, 25, 255) if edge else (220, 185, 75, 255)
    return pix

def make_shield():
    W, H = 128, 128
    pix = [[(0, 0, 0, 0) for _ in range(W)] for _ in range(H)]
    for y in range(H):
        for x in range(W):
            dx = x - 64
            dy = y - 64
            dist = math.sqrt(dx * dx + dy * dy)
            if dist <= 52:
                if dist > 46:
                    pix[y][x] = (75, 85, 100, 255)
                elif dist > 42:
                    pix[y][x] = (175, 190, 205, 255)
                elif dist > 40:
                    pix[y][x] = (60, 70, 85, 255)
                elif dist <= 16:
                    if dist <= 12:
                        pix[y][x] = (220, 235, 250, 255) if (dx - dy < 0) else (130, 145, 165, 255)
                    else:
                        pix[y][x] = (70, 80, 95, 255)
                else:
                    plank = (x // 14) % 2
                    if plank == 0:
                        pix[y][x] = (135, 85, 45, 255) if dy < 0 else (110, 65, 35, 255)
                    else:
                        pix[y][x] = (155, 100, 55, 255) if dy < 0 else (125, 75, 40, 255)
                    if x % 14 == 0:
                        pix[y][x] = (50, 30, 15, 255)
    return pix

def make_armor():
    W, H = 128, 128
    pix = [[(0, 0, 0, 0) for _ in range(W)] for _ in range(H)]
    for y in range(20, 108):
        for x in range(24, 104):
            dx = abs(x - 64)
            top_bound = 20 + dx * 0.4 if dx < 22 else 32
            bot_bound = 104 - dx * 0.3
            if top_bound <= y <= bot_bound and dx < 36:
                if dx < 14 and y < 38:
                    continue
                edge = (y == int(top_bound) or y == int(bot_bound) or dx >= 34)
                if edge:
                    pix[y][x] = (50, 60, 75, 255)
                elif y % 12 == 0 or dx % 14 == 0:
                    pix[y][x] = (80, 95, 115, 255)
                elif x < 64:
                    pix[y][x] = (180, 200, 220, 255)
                else:
                    pix[y][x] = (120, 140, 165, 255)
    for y in range(45, 85):
        for x in range(50, 79):
            if abs(x - 64) + abs(y - 65) < 14:
                pix[y][x] = (220, 185, 60, 255) if abs(x - 64) + abs(y - 65) < 12 else (110, 90, 25, 255)
    return pix

def make_potion(color_base, is_large=False):
    W, H = 128, 128
    pix = [[(0, 0, 0, 0) for _ in range(W)] for _ in range(H)]
    r_max = 38 if is_large else 32
    cy = 76 if is_large else 78
    for y in range(H):
        for x in range(W):
            dx = x - 64
            dy = y - cy
            dist = math.sqrt(dx * dx + dy * dy)
            neck = (abs(dx) <= (14 if is_large else 11)) and (34 <= y <= cy - 18)
            cork = (abs(dx) <= (16 if is_large else 13)) and (22 <= y < 34)
            cork_top = (abs(dx) <= (18 if is_large else 15)) and (18 <= y < 22)
            
            if cork or cork_top:
                edge = abs(dx) >= (15 if is_large else 12) or y in (18, 22, 33)
                pix[y][x] = (90, 55, 25, 255) if edge else (175, 125, 75, 255)
            elif neck or dist <= r_max:
                edge = (dist >= r_max - 3) or (neck and abs(dx) >= (12 if is_large else 9))
                liquid = (y >= (54 if is_large else 58)) and (dist < r_max - 3)
                if edge:
                    pix[y][x] = (170, 210, 240, 200) if dx < 0 and dy < 0 else (70, 100, 130, 220)
                elif liquid:
                    br, bg, bb = color_base
                    hl = (dx < -6 and dy < -6)
                    if hl:
                        pix[y][x] = (min(255, br + 70), min(255, bg + 70), min(255, bb + 70), 250)
                    else:
                        pix[y][x] = (br, bg, bb, 235)
                else:
                    pix[y][x] = (220, 240, 255, 100) if (dx < 0 and dy < -10) else (20, 35, 55, 120)
    return pix

def make_scroll():
    W, H = 128, 128
    pix = [[(0, 0, 0, 0) for _ in range(W)] for _ in range(H)]
    for y in range(24, 104):
        for x in range(30, 98):
            edge = (x in (30, 31, 96, 97) or y in (24, 25, 102, 103))
            roll_l = x < 42
            roll_r = x > 86
            ribbon = 60 <= y <= 68
            if ribbon:
                pix[y][x] = (200, 45, 45, 255) if not edge else (120, 25, 25, 255)
            elif edge:
                pix[y][x] = (120, 95, 60, 255)
            elif roll_l or roll_r:
                pix[y][x] = (225, 205, 160, 255) if x % 3 == 0 else (195, 175, 130, 255)
            else:
                pix[y][x] = (245, 230, 195, 255)
                if 36 <= y <= 90 and y % 8 in (0, 1) and 46 <= x <= 82:
                    pix[y][x] = (90, 70, 50, 255)
    return pix

def make_bag():
    W, H = 128, 128
    pix = [[(0, 0, 0, 0) for _ in range(W)] for _ in range(H)]
    for y in range(35, 105):
        for x in range(28, 100):
            dx = abs(x - 64)
            curve = (y - 35) * 0.15
            if dx <= 32 + curve:
                edge = (y in (35, 104) or dx >= int(30 + curve))
                strap = 50 <= x <= 56 or 72 <= x <= 78
                buckle = (50 <= x <= 56 or 72 <= x <= 78) and (68 <= y <= 74)
                if buckle:
                    pix[y][x] = (230, 195, 75, 255)
                elif strap:
                    pix[y][x] = (75, 40, 20, 255)
                elif edge:
                    pix[y][x] = (55, 30, 15, 255)
                elif y < 55:
                    pix[y][x] = (150, 95, 50, 255)
                else:
                    pix[y][x] = (120, 75, 40, 255) if dx > 0 else (140, 85, 45, 255)
    for x in range(48, 81):
        for y in range(22, 36):
            if abs(x - 64) <= 14 and (y == 22 or abs(x - 64) >= 12):
                pix[y][x] = (85, 50, 25, 255)
    return pix

base = 'assets/pictures'
save_png(f'{base}/epee/epee.png', 128, 128, make_sword())
save_png(f'{base}/bouclier/bouclier.png', 128, 128, make_shield())
save_png(f'{base}/armure/armure.png', 128, 128, make_armor())
save_png(f'{base}/potion_soin/potion_soin.png', 128, 128, make_potion((210, 45, 45)))
save_png(f'{base}/potion_mana/potion_mana.png', 128, 128, make_potion((45, 135, 240)))
save_png(f'{base}/grande_potion/grande_potion.png', 128, 128, make_potion((235, 185, 40), True))
save_png(f'{base}/parchemin/parchemin.png', 128, 128, make_scroll())
save_png(f'{base}/sac/sac.png', 128, 128, make_bag())
print("All 8 PNG icons generated successfully!")
