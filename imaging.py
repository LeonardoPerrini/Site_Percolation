#Leonardo Perrini

import numpy as np 
import PIL.Image

X = np.loadtxt("percolation.dat")
#s = np.loadtxt("percolation_spin.dat")

def render_hsv(x):
    x_norm = x.reshape(list(x.shape)+[1])  #normalizza i valori di x
    img = np.concatenate([5069809*x_norm, 255*np.ones_like(x_norm), 255*np.ones_like(x_norm)], 2)   #3334572 con 1000 s, 2385 con 100 s
    img = np.uint8(np.mod(img, 360))     #riduce i valori della hue modulo 360
    img = np.uint8(np.clip(img, 0, 360))
    return PIL.Image.fromarray(img,'HSV')

def render_rgb(x):
    x_norm = x.reshape(list(x.shape)+[1])    
    img = np.concatenate([2550*x_norm+65, 3152*x_norm+30, 1758*x_norm+200], 2) 
    img = np.uint8(np.mod(img, 255))
    img = np.uint8(np.clip(img, 0, 255))   
    return PIL.Image.fromarray(img, 'RGB')

def render_rgb2(x):
    x_norm = x.reshape(list(x.shape)+[1])    
    img = np.concatenate([87467297974*x_norm+65, 637479018642*x_norm+30, 318717268472*x_norm+200], 2) 
    img = np.uint8(np.mod(img, 255))
    img = np.uint8(np.clip(img, 0, 255))   
    return PIL.Image.fromarray(img, 'RGB')

render_rgb2(X).save('perc.png')  
