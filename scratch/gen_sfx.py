# Generateur d'effets sonores - Python pur (wave + math + random)
import math, random, struct, wave, os

SR = 44100
random.seed(42)
os.makedirs("assets/sounds", exist_ok=True)

def save(nom, samples):
    path = f"assets/sounds/{nom}.wav"
    with wave.open(path, "w") as f:
        f.setnchannels(1)
        f.setsampwidth(2)
        f.setframerate(SR)
        frames = bytearray()
        for s in samples:
            v = max(-1.0, min(1.0, s))
            frames += struct.pack("<h", int(v * 32000))
        f.writeframes(bytes(frames))
    print(f"OK {path} ({len(samples)/SR:.2f}s)")

def blip(freq, duree, decay=18.0, harmoniques=((1, 1.0),)):
    n = int(SR * duree)
    out = []
    for i in range(n):
        t = i / SR
        e = (t / 0.004 if t < 0.004 else 1.0) * math.exp(-decay * t)
        v = sum(a * math.sin(2 * math.pi * freq * k * t) for k, a in harmoniques)
        out.append(0.8 * e * v)
    return out

def melodie(notes, duree_note, gap=0.02, decay=7.0, vol=0.7):
    out = []
    for f in notes:
        s = blip(f, duree_note, decay, ((1, 1.0), (2, 0.35), (3, 0.12)))
        out += s
        out += [0.0] * int(SR * gap)
    return [v * vol for v in out]

# 1. Coup d'epee : bruit + impact grave
n = int(SR * 0.20)
s = []
for i in range(n):
    t = i / SR
    bruit = random.uniform(-1, 1) * math.exp(-t * 28)
    thump = math.sin(2 * math.pi * 110 * t) * math.exp(-t * 22) * 0.8
    s.append(0.75 * bruit + thump)
save("coup", s)

# 2. Coup critique : clang metallique aigu
n = int(SR * 0.30)
s = []
for i in range(n):
    t = i / SR
    metal = (math.sin(2 * math.pi * 1800 * t) + 0.7 * math.sin(2 * math.pi * 2700 * t)
             + 0.5 * math.sin(2 * math.pi * 3500 * t)) * math.exp(-t * 16)
    bruit = random.uniform(-1, 1) * math.exp(-t * 45) * 0.5
    s.append(0.4 * metal + bruit)
save("critique", s)

# 3. Sort magique : balayage ascendant scintillant
n = int(SR * 0.45)
s = []
for i in range(n):
    t = i / SR
    f = 450 + 750 * (t / 0.45) + 40 * math.sin(2 * math.pi * 9 * t)
    e = math.sin(math.pi * min(1.0, t / 0.45)) ** 0.7
    v = math.sin(2 * math.pi * f * t) + 0.3 * math.sin(2 * math.pi * f * 2.02 * t)
    s.append(0.55 * e * v)
save("sort", s)

# 4. Potion : glouglou descendant
s = []
for j, f in enumerate((620, 460, 330)):
    b = blip(f, 0.09, 26, ((1, 1.0),))
    s += b + [0.0] * int(SR * 0.03)
n = len(s)
for i in range(n):
    s[i] += 0.10 * random.uniform(-1, 1) * math.exp(-6 * (i / SR))
save("potion", s)

# 5. Bouclier : clang lourd
n = int(SR * 0.40)
s = []
for i in range(n):
    t = i / SR
    clang = (math.sin(2 * math.pi * 880 * t) + 0.6 * math.sin(2 * math.pi * 1320 * t)
             + 0.4 * math.sin(2 * math.pi * 2100 * t)) * math.exp(-t * 10)
    attaque = random.uniform(-1, 1) * math.exp(-t * 60) * 0.6
    s.append(0.45 * clang + attaque)
save("bouclier", s)

# 6. Fuite : souffle descendant
n = int(SR * 0.32)
s = []
for i in range(n):
    t = i / SR
    f = 900 - 600 * (t / 0.32)
    v = math.sin(2 * math.pi * f * t) + 0.5 * random.uniform(-1, 1) * math.exp(-t * 9)
    e = math.sin(math.pi * min(1.0, t / 0.32))
    s.append(0.45 * e * v)
save("fuite", s)

# 7. Victoire : fanfare ascendante
save("victoire", melodie((523, 659, 784, 1046), 0.17, 0.03, 6.0, 0.75))

# 8. Defaite : descente minceure
save("defaite", melodie((220, 175, 147), 0.30, 0.05, 4.5, 0.70))

# 9. Level up : arpege brillant rapide
save("niveau", melodie((523, 659, 784, 1046, 1318), 0.12, 0.015, 8.0, 0.72))

# 10. Pieces d'or : deux pings clairs
s = blip(1800, 0.14, 20, ((1, 1.0), (2.76, 0.4)))
s += [0.0] * int(SR * 0.06)
s += [v * 0.9 for v in blip(2400, 0.14, 20, ((1, 1.0), (2.76, 0.4)))]
save("piece", s)

# 11. Rugissement de boss : growl grave + souffle
n = int(SR * 1.5)
s = []
brown = 0.0
for i in range(n):
    t = i / SR
    wob = 1 + 0.13 * math.sin(2 * math.pi * 2.7 * t) + 0.06 * math.sin(2 * math.pi * 6.3 * t)
    f = 65 * wob
    harm = sum(math.sin(2 * math.pi * f * k * t) / k for k in range(1, 8))
    brown += random.uniform(-1, 1) * 0.02
    brown *= 0.997
    e = min(1.0, t / 0.22) * math.exp(-1.7 * max(0.0, t - 0.22))
    s.append((0.55 * harm + 0.65 * brown) * e)
save("rugissement", s)

# 12. Clic d'interface
save("clic", blip(1200, 0.06, 80, ((1, 1.0),)))

print("Tous les sons generes !")
