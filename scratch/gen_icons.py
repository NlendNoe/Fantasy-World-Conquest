# Generateur d'icones d'objets (64x64, RGBA) - Python pur
import zlib, struct, math, random, os

S = 64
random.seed(3)

def chunk(t, d):
    return struct.pack(">I", len(d)) + t + d + struct.pack(">I", zlib.crc32(t + d))

def save(path, px):
    raw = b"".join(b"\x00" + bytes(v for p in row for v in p) for row in px)
    data = (b"\x89PNG\r\n\x1a\n"
            + chunk(b"IHDR", struct.pack(">IIBBBBB", S, S, 8, 6, 0, 0, 0))
            + chunk(b"IDAT", zlib.compress(raw, 6))
            + chunk(b"IEND", b""))
    os.makedirs(os.path.dirname(path), exist_ok=True)
    open(path, "wb").write(data)
    print("OK", path)

def canvas():
    return [[[0, 0, 0, 0] for _ in range(S)] for _ in range(S)]

def px_(px, x, y, c):
    if 0 <= x < S and 0 <= y < S:
        px[y][x] = list(c)

def rect(px, x0, y0, x1, y1, c):
    for y in range(int(y0), int(y1)):
        for x in range(int(x0), int(x1)):
            px_(px, x, y, c)

def disque(px, cx, cy, r, c):
    for y in range(S):
        for x in range(S):
            if math.hypot(x - cx, y - cy) <= r:
                px_(px, x, y, c)

# ---------- 1. EPEE ----------
px = canvas()
rect(px, 28, 4, 35, 40, (200, 205, 215, 255))       # lame
rect(px, 28, 4, 31, 40, (235, 240, 250, 255))       # reflet
px_(px, 31, 6, (255, 255, 255, 255))
rect(px, 20, 40, 43, 45, (218, 165, 60, 255))       # garde
rect(px, 30, 45, 33, 55, (110, 70, 35, 255))        # manche
disque(px, 31.5, 58, 4, (218, 165, 60, 255))        # pommeau
rect(px, 33, 5, 35, 38, (150, 155, 170, 255))
save("assets/pictures/epee/epee.png", px)

# ---------- 2. BOUCLIER ----------
px = canvas()
for y in range(8, 56):
    t = (y - 8) / 48.0
    demi = 20 * (1 - t * t * 0.9) if t < 0.7 else max(2, int(9 * (1 - (t - 0.7) / 0.3)))
    demi = int(demi)
    rect(px, 32 - demi, y, 32 + demi, y + 1, (120, 128, 145, 255))
    px_(px, 32 - demi, y, (75, 82, 100, 255))
    px_(px, 32 + demi - 1, y, (75, 82, 100, 255))
disque(px, 32, 30, 7, (218, 165, 60, 255))          # umbo dore
disque(px, 30, 28, 2, (255, 220, 130, 255))
rect(px, 28, 12, 35, 17, (90, 60, 30, 255))         # attache cuir
save("assets/pictures/bouclier/bouclier.png", px)

# ---------- 3. ARMURE ----------
px = canvas()
for y in range(12, 54):
    t = (y - 12) / 42.0
    gauche = 14 + 4 * t
    droite = 49 - 4 * t
    rect(px, gauche, y, droite, y + 1, (170, 178, 195, 255))
    px_(px, int(gauche), y, (110, 118, 135, 255))
    px_(px, int(droite) - 1, y, (110, 118, 135, 255))
rect(px, 26, 8, 37, 16, (120, 128, 145, 255))       # encolure
rect(px, 16, 40, 47, 46, (90, 55, 30, 255))         # ceinture
rect(px, 29, 40, 34, 46, (218, 165, 60, 255))       # boucle
rect(px, 20, 16, 26, 36, (215, 222, 238, 255))      # reflet
rect(px, 14, 12, 20, 30, (150, 158, 175, 255))      # epaulette G
rect(px, 43, 12, 49, 30, (150, 158, 175, 255))      # epaulette D
save("assets/pictures/armure/armure.png", px)

# ---------- fiole generique ----------
def fiole(couleur, grande=False):
    px = canvas()
    cy = 34 if grande else 38
    r = 19 if grande else 15
    haut = 8 if grande else 12
    rect(px, 27, haut, 37, cy - r + 6, (190, 200, 215, 200))     # col
    rect(px, 25, haut - 4, 39, haut + 4, (110, 70, 35, 255))     # bouchon
    disque(px, 32, cy, r, (200, 212, 228, 210))                  # verre
    disque(px, 32, cy + r * 0.35, r * 0.82, couleur)             # liquide
    disque(px, 32, cy + r * 0.45, r * 0.72, couleur)
    disque(px, 26, cy - r * 0.35, 3, (255, 255, 255, 190))       # reflet
    return px

save("assets/pictures/potion_soin/potion_soin.png", fiole((215, 45, 55, 235)))
save("assets/pictures/potion_mana/potion_mana.png", fiole((45, 110, 245, 235)))
save("assets/pictures/grande_potion/grande_potion.png", fiole((215, 45, 55, 245), grande=True))

# ---------- 4. PARCHEMIN ----------
px = canvas()
rect(px, 12, 18, 52, 48, (225, 205, 150, 255))
rect(px, 12, 18, 52, 21, (195, 172, 118, 255))
rect(px, 12, 45, 52, 48, (195, 172, 118, 255))
rect(px, 8, 16, 14, 50, (205, 182, 128, 255))        # rouleau gauche
rect(px, 50, 16, 56, 50, (205, 182, 128, 255))       # rouleau droit
rect(px, 20, 25, 44, 27, (140, 130, 110, 255))       # lignes "ecriture"
rect(px, 20, 31, 44, 33, (140, 130, 110, 255))
rect(px, 20, 37, 36, 39, (140, 130, 110, 255))
rect(px, 28, 13, 36, 19, (190, 60, 60, 255))         # ruban rouge
save("assets/pictures/parchemin/parchemin.png", px)

# ---------- 5. SAC ----------
px = canvas()
disque(px, 32, 40, 19, (145, 100, 52, 255))
disque(px, 32, 42, 18, (130, 88, 44, 255))
rect(px, 24, 16, 40, 28, (110, 74, 38, 255))         # cou du sac
rect(px, 22, 24, 42, 29, (200, 160, 90, 255))        # corde
rect(px, 22, 24, 42, 26, (225, 190, 115, 255))
disque(px, 32, 22, 8, (95, 62, 30, 255))             # ouverture
rect(px, 22, 40, 42, 42, (110, 74, 38, 255))         # pli
disque(px, 25, 34, 3, (170, 125, 70, 255))           # reflet
save("assets/pictures/sac/sac.png", px)

print("Icones generees !")
