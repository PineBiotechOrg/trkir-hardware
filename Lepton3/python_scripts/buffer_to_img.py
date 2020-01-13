import sys
import os
import time
import numpy as np
import cv2

prev_txts = []
while True:
    txts = [f for f in os.listdir() if '.txt' in f and f not in prev_txts]
    txts.sort(key=lambda x: int(x.split('.')[0]))

    for txt in txts:
        print(txt)
        try:
            with open(txt, 'r') as f:
                img = f.readlines()[0]

            img = img.split(' ')[:-1]
            img = np.array(img).astype(np.uint8)

            img = np.array(np.array_split(img, len(img) // 164))
            img = img[:, 4:]

            new_img = []
            for row in range(0, 240, 2):
                new_row = []
                for part in range(2):
                    for pix in range(0, 160, 2):
                        new_row.append((img[row + part][pix] << 8) | img[row+ part][pix + 1])
                new_img.append(new_row)

            img = np.array(new_img)
            img = (img - img.min()) /(img.max() - img.min())

            cv2.imshow('img', img)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        except:
            import traceback
            traceback.print_exc()
            continue

    prev_txts += txts[:]
    time.sleep(0.1)
