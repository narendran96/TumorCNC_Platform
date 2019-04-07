from PlatformControl import ControlPlatform
import time
from tkinter import *

class gui:
    def __init__(self):
        self.PF = ControlPlatform()

        window = Tk()
        window.title("Platform Controller")

        text = Label(window, text="ROTATION (degrees) AND TRANSLATION (in)")
        text.grid(row=0, columnspan=2, sticky=N)

        self.x1 = Entry(window)
        self.x1.insert(END, '0')
        self.y1 = Entry(window)
        self.y1.insert(END, '0')
        self.z1 = Entry(window)
        self.z1.insert(END, '0')

        self.x2 = Entry(window)
        self.x2.insert(END, '0')
        self.y2 = Entry(window)
        self.y2.insert(END, '0')
        self.z2 = Entry(window)
        self.z2.insert(END, '0')

        xa_lab = Label(window, text="X rotation:")
        xa_lab.grid(row=1, column=0)
        ya_lab = Label(window, text="Y rotation")
        ya_lab.grid(row=2, column=0)
        za_lab = Label(window, text="Z rotation")
        za_lab.grid(row=3, column=0)

        xt_lab = Label(window, text="X translation:")
        xt_lab.grid(row=4, column=0)
        yt_lab = Label(window, text="Y translation:")
        yt_lab.grid(row=5, column=0)
        zt_lab = Label(window, text="Z translation:")
        zt_lab.grid(row=6, column=0)

        self.x1.grid(row=1, column = 1)
        self.y1.grid(row=2, column = 1)
        self.z1.grid(row=3, column = 1)
        self.x2.grid(row=4, column = 1)
        self.y2.grid(row=5, column = 1)
        self.z2.grid(row=6, column = 1)

        collect = Button(window, text=" ROTATE | TRANSLATE ", command=self.control)
        collect.grid(row=8, columnspan=2, sticky=W+E)
        set = Button(window, text=" RESET ", command=self.Reset)
        set.grid(row=9, columnspan=2, sticky=W+E)
        window.mainloop()

    def control(self):
        self.PF.rotate_translate(self.x1.get(), self.y1.get(), self.z1.get(), self.x2.get(), self.y2.get(), self.z2.get())
    def Reset(self):
        self.x1.delete(0, 'end')
        self.y1.delete(0, 'end')
        self.z1.delete(0, 'end')
        self.x2.delete(0, 'end')
        self.y2.delete(0, 'end')
        self.z2.delete(0, 'end')
        self.PF.reset()

if __name__ == '__main__':
    gui()
