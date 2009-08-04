"""
Offline DT Calibration GUI 
"""

from Tkinter import *

class App(Frame):
    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.master.title("DT Calibration Offline GUI")
        self.master.maxsize(1000,600)
        self.pack()
        self.createWidgets()

    def createWidgets(self):
        self.frame_left = Frame(self,height=600,width=400)
        self.frame_left.pack(side=LEFT)

        self.frame_top = Frame(self.frame_left,height=100,width=400)
        self.frame_top.pack()
        Label(self.frame_top,text="Run number:").pack(side=LEFT)
        self.entry_run = Entry(self.frame_top)
        self.entry_run.pack(side=RIGHT)
        self.input_run = StringVar()
        self.input_run.set("input run number")
        self.entry_run["textvariable"] = self.input_run
        self.entry_run.bind('<Key-Return>',self.appendText)

        self.button_run = Button(self.frame_left,text="RUN",fg="red",command=self.run)
        self.button_run.pack()
        self.button_quit = Button(self.frame_left,text="QUIT",fg="red",command=self.quit)
        self.button_quit.pack()

        self.frame_right = Frame(self,height=600,width=600)
        self.frame_right.pack()

        self.text_sbar = Scrollbar(self.frame_right)
        self.out_text = Text(self.frame_right,height=600,width=600,bg="white")
        self.text_sbar.config(command=self.out_text.yview) 
        self.out_text.config(yscrollcommand=self.text_sbar.set)
        self.text_sbar.pack(side=RIGHT,fill=Y)
        self.out_text.pack(side=LEFT,fill=BOTH)

        """
        self.hi_there = Button(self)
        self.hi_there["text"] = "Hello",
        self.hi_there["command"] = self.say_hi

        self.hi_there.pack({"side": "left"})

        self.entrythingy = Entry()
        self.entrythingy.pack({"side":"right"})
        
        self.contents = StringVar()
        self.contents.set("this is a variable")
        self.entrythingy["textvariable"] = self.contents

        self.entrythingy.bind('<Key-Return>',
                              self.print_contents)
        """

    def appendText(self, event):
        text = self.input_run.get()
        self.out_text.insert(END,text) 
        self.out_text.see(END)
        #self.out_text.focus()

    def print_contents(self, event):
        print "hi. contents of entry is now ---->", \
              self.contents.get()

    def run(self):
        self.out_text.insert(END,"Starting calibration of Run %s\n" % self.input_run.get())
        self.out_text.see(END)

if __name__ == '__main__':
    root = Tk()
    app = App(master=root)
    app.mainloop()
    #root.destroy()
