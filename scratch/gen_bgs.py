# Generateur de fonds d'ecran - Python pur (PNG via zlib)
import zlib, struct, math, random, os

W, H = 960, 540
random.seed(7)
os.makedirs("assets/backgrounds", exist_ok=True)

def chunk(t, d):
    return struct.pack(">I", len(d)) + t + d + struct.pack(">I", zlib.crc32(t + d))

def save(path, rows):
    raw = b"".join(b"\x00" + bytes(v for px in row for v in px) for row in rows)
    data = (b"\x89PNG\r\n\x1a\n"
            + chunk(b"IHDR", struct.pack(">IIBBBBB", W, H, 8, 2, 0, 0, 0))
            + chunk(b"IDAT", zlib.compress(raw, 6))
            + chunk(b"IEND", b""))
    open(path, "wb").write(data)
    print("OK", path)

def melange(a, b, t):
    return [int(a[i] + (b[i] - a[i]) * t) for i in range(3)]

def base(top, bot):
    rows = []
    for y in range(H):
        c = melange(top, bot, y / (H - 1))
        rows.append([list(c) for _ in range(W)])
    return rows

def rect(rows, x0, y0, x1, y1, c):
    for y in range(max(0, int(y0)), min(H, int(y1))):
        row = rows[y]
        for x in range(max(0, int(x0)), min(W, int(x1))):
            row[x][0], row[x][1], row[x][2] = c

def disque(rows, cx, cy, r, c, force=1.0):
    for y in range(max(0, int(cy - r)), min(H, int(cy + r))):
        for x in range(max(0, int(cx - r)), min(W, int(cx + r))):
            d = math.hypot(x - cx, y - cy)
            if d < r:
                f = (1 - d / r) * force
                px = rows[y][x]
                for i in range(3):
                    px[i] = int(min(255, px[i] + (c[i] - px[i]) * f))

def silhouette(rows, fn, c, ymin=0):
    for x in range(W):
        hy = fn(x)
        for y in range(max(int(ymin), int(hy)), H):
            rows[y][x][0], rows[y][x][1], rows[y][x][2] = c

def points(rows, n, c, ymax=H):
    for _ in range(n):
        x = random.randrange(W)
        y = random.randrange(int(ymax))
        rows[y][x][0], rows[y][x][1], rows[y][x][2] = c

def arbre(rows, x0, sol, haut, c):
    rect(rows, x0 - 3, sol - haut * 0.35, x0 + 3, sol, c)
    cy = sol - haut * 0.55
    r = haut * 0.30
    disque(rows, x0, cy, r, c, 1.0)
    disque(rows, x0 - r * 0.6, cy + r * 0.3, r * 0.7, c, 1.0)
    disque(rows, x0 + r * 0.6, cy + r * 0.3, r * 0.7, c, 1.0)

def chateau(rows, x0, sol, w, h, c, fenetre=(255, 210, 90)):
    rect(rows, x0, sol - h, x0 + w, sol, c)
    for tx in (x0 - 8, x0 + w - 8):
        rect(rows, tx, sol - h * 1.35, tx + 14, sol, c)
        p = 0
        for px in range(int(tx), int(tx) + 14, 4):
            rect(rows, px, sol - h * 1.35 - 6, px + 2, sol - h * 1.35, c)
            p += 1
    for fy in range(int(sol - h + 10), int(sol - 10), 22):
        for fx in range(int(x0 + 8), int(x0 + w - 8), 26):
            if random.random() < 0.5:
                rect(rows, fx, fy, fx + 4, fy + 7, fenetre)

# ---------- MENU PRINCIPAL : nuit etoilee, chateau au sommet ----------
rows = base((8, 10, 30), (30, 25, 60))
points(rows, 220, (240, 240, 255), int(H * 0.62))
disque(rows, W * 0.78, H * 0.16, 60, (200, 210, 255), 0.9)
disque(rows, W * 0.78, H * 0.16, 130, (120, 140, 220), 0.35)
silhouette(rows, lambda x: H * 0.62 + 30 * math.sin(x * 0.008) + 18 * math.sin(x * 0.021 + 2), (18, 20, 42))
silhouette(rows, lambda x: H * 0.74 + 22 * math.sin(x * 0.012 + 1), (11, 12, 28))
chateau(rows, W * 0.40, H * 0.62 + 30 * math.sin(W * 0.008 * 0.5) , 150, 90, (16, 16, 36))
save("assets/backgrounds/fond_menu.png", rows)

# ---------- CITE : crepuscule chaleureux ----------
rows = base((255, 150, 80), (60, 30, 70))
disque(rows, W * 0.5, H * 0.42, 70, (255, 230, 160), 0.85)
silhouette(rows, lambda x: H * 0.68 + 14 * math.sin(x * 0.02), (40, 25, 50))
for i in range(9):
    bx = 40 + i * 105 + random.randrange(20)
    bh = 50 + random.randrange(55)
    rect(rows, bx, H * 0.72 - bh, bx + 70, H * 0.72, (30, 20, 42))
    rect(rows, bx + 20, H * 0.72 - bh - 18, bx + 40, H * 0.72 - bh, (30, 20, 42))
    for fy in range(int(H * 0.72 - bh + 8), int(H * 0.72 - 8), 16):
        for fx in range(bx + 8, bx + 60, 18):
            if random.random() < 0.45:
                rect(rows, fx, fy, fx + 4, fy + 6, (255, 200, 90))
rect(rows, 0, H * 0.72, W, H, (24, 16, 34))
save("assets/backgrounds/fond_cite.png", rows)

# ---------- BOUTIQUE : interieur en bois ----------
rows = base((70, 45, 25), (30, 18, 12))
disque(rows, W * 0.18, H * 0.30, 60, (255, 180, 80), 0.55)
disque(rows, W * 0.82, H * 0.30, 60, (255, 180, 80), 0.55)
for sy in (H * 0.42, H * 0.62, H * 0.80):
    rect(rows, W * 0.06, sy, W * 0.94, sy + 12, (20, 12, 8))
    x = W * 0.10
    while x < W * 0.90:
        hw = 16 + random.randrange(14)
        hh = 20 + random.randrange(22)
        c = (110 + random.randrange(50), 70 + random.randrange(30), 40)
        rect(rows, x, sy - hh, x + hw, sy, c)
        x += hw + 14
rect(rows, 0, H * 0.90, W, H, (16, 10, 7))
save("assets/backgrounds/fond_boutique.png", rows)

# ---------- COMBAT : arene sombre ----------
rows = base((45, 22, 28), (14, 8, 12))
disque(rows, W * 0.5, H * 0.30, 180, (120, 40, 40), 0.30)
rect(rows, 0, H * 0.66, W, H, (22, 14, 18))
rect(rows, 0, H * 0.66, W, H * 0.66 + 4, (90, 50, 55))
rect(rows, W * 0.04, H * 0.20, W * 0.10, H * 0.70, (30, 20, 26))
rect(rows, W * 0.90, H * 0.20, W * 0.96, H * 0.70, (30, 20, 26))
rect(rows, W * 0.03, H * 0.16, W * 0.11, H * 0.22, (36, 24, 30))
rect(rows, W * 0.89, H * 0.16, W * 0.97, H * 0.22, (36, 24, 30))
save("assets/backgrounds/fond_combat.png", rows)

# ---------- VICTOIRE : rayonnement dore ----------
rows = base((60, 40, 15), (20, 12, 6))
disque(rows, W * 0.5, H * 0.38, 240, (255, 210, 90), 0.75)
disque(rows, W * 0.5, H * 0.38, 420, (200, 150, 50), 0.35)
for k in range(14):
    a = k * math.pi / 7
    for rr in range(120, 460, 6):
        x = int(W * 0.5 + rr * math.cos(a))
        y = int(H * 0.38 + rr * math.sin(a))
        if 0 <= x < W and 0 <= y < H:
            px = rows[y][x]
            for i in range(3):
                px[i] = int(min(255, px[i] + 14))
save("assets/backgrounds/fond_victoire.png", rows)

# ---------- ZONE 1 : foret des anciens ----------
rows = base((120, 180, 120), (30, 55, 35))
disque(rows, W * 0.22, H * 0.20, 55, (250, 245, 190), 0.8)
for _ in range(7):
    arbre(rows, random.randrange(20, W - 20), H * 0.80 + random.randrange(30), 130 + random.randrange(90), (16, 34, 22))
for _ in range(6):
    arbre(rows, random.randrange(20, W - 20), H * 0.95 + random.randrange(20), 180 + random.randrange(100), (10, 24, 15))
rect(rows, 0, H * 0.86, W, H, (20, 40, 26))
points(rows, 40, (200, 255, 120), int(H * 0.85))
save("assets/backgrounds/fond_zone1.png", rows)

# ---------- ZONE 2 : montagnes du destin ----------
rows = base((110, 160, 220), (200, 220, 240))
disque(rows, W * 0.80, H * 0.15, 45, (255, 255, 240), 0.9)
silhouette(rows, lambda x: H * 0.52 + 60 * math.sin(x * 0.006) + 30 * math.sin(x * 0.017 + 3), (70, 90, 130))
silhouette(rows, lambda x: H * 0.62 + 50 * math.sin(x * 0.009 + 1) + 25 * math.sin(x * 0.023), (45, 60, 95))
silhouette(rows, lambda x: H * 0.74 + 40 * math.sin(x * 0.013 + 4) + 18 * math.sin(x * 0.03 + 1), (28, 38, 62))
for x in range(0, W, 3):
    y1 = H * 0.52 + 60 * math.sin(x * 0.006) + 30 * math.sin(x * 0.017 + 3)
    if y1 < H * 0.48:
        for y in range(int(y1), int(y1) + 14):
            rows[y][x] = [225, 235, 250]
rect(rows, 0, H * 0.86, W, H, (35, 45, 65))
save("assets/backgrounds/fond_zone2.png", rows)

# ---------- ZONE 3 : desert maudit ----------
rows = base((255, 200, 120), (180, 120, 60))
disque(rows, W * 0.5, H * 0.24, 75, (255, 245, 200), 0.95)
disque(rows, W * 0.5, H * 0.24, 160, (255, 220, 140), 0.35)
silhouette(rows, lambda x: H * 0.60 + 26 * math.sin(x * 0.008), (200, 150, 85))
silhouette(rows, lambda x: H * 0.72 + 34 * math.sin(x * 0.006 + 2), (165, 115, 60))
silhouette(rows, lambda x: H * 0.86 + 24 * math.sin(x * 0.011 + 5), (130, 88, 45))
points(rows, 25, (240, 225, 180), int(H * 0.55))
save("assets/backgrounds/fond_zone3.png", rows)

# ---------- ZONE 4 : royaume des ombres ----------
rows = base((25, 15, 40), (8, 5, 14))
disque(rows, W * 0.70, H * 0.18, 50, (220, 60, 60), 0.9)
disque(rows, W * 0.70, H * 0.18, 120, (140, 30, 30), 0.30)
for i in range(8):
    sx = 30 + i * 125 + random.randrange(30)
    sh = 120 + random.randrange(140)
    w = 24 + random.randrange(20)
    rect(rows, sx, H * 0.80 - sh, sx + w, H * 0.80, (12, 8, 22))
    rect(rows, sx - 4, H * 0.80 - sh - 10, sx + w + 4, H * 0.80 - sh, (12, 8, 22))
rect(rows, 0, H * 0.80, W, H, (5, 3, 10))
points(rows, 60, (180, 60, 80), int(H * 0.75))
save("assets/backgrounds/fond_zone4.png", rows)

# ---------- ZONE 5 : domaine vampirique ----------
rows = base((60, 15, 30), (15, 4, 12))
disque(rows, W * 0.24, H * 0.20, 48, (230, 225, 235), 0.85)
disque(rows, W * 0.24, H * 0.20, 110, (150, 100, 130), 0.30)
chateau(rows, W * 0.52, H * 0.78, 300, 190, (18, 8, 16))
rect(rows, 0, H * 0.78, W, H, (10, 4, 10))
for _ in range(12):
    bx, by = random.randrange(W), random.randrange(int(H * 0.5))
    rect(rows, bx, by, bx + 5, by + 2, (15, 8, 14))
    rect(rows, bx - 3, by - 2, bx, by + 2, (15, 8, 14))
    rect(rows, bx + 5, by - 2, bx + 8, by + 2, (15, 8, 14))
points(rows, 30, (200, 60, 90), int(H * 0.70))
save("assets/backgrounds/fond_zone5.png", rows)

# ---------- ZONE 6 : foret maudite finale ----------
rows = base((45, 60, 70), (10, 18, 25))
disque(rows, W * 0.78, H * 0.20, 40, (150, 255, 200), 0.6)
for _ in range(8):
    arbre(rows, random.randrange(20, W - 20), H * 0.85 + random.randrange(25), 150 + random.randrange(110), (8, 18, 20))
rect(rows, 0, H * 0.88, W, H, (6, 12, 15))
points(rows, 55, (120, 255, 190), int(H * 0.88))
points(rows, 30, (90, 140, 160), int(H * 0.60))
save("assets/backgrounds/fond_zone6.png", rows)

print("Tous les fonds generes !")
