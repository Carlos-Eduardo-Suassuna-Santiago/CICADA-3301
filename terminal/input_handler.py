import sys
import os

#====================
#WINDOWS
#====================

if os.name == 'nt':

    import msvcrt

    def getch_char():
        return msvcrt.getch()

#====================
#LINUX / MAC
#====================   

else:

    import tty
    import termios

    def getch_char():

        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)

        try:
            tty.setraw(fd)
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch.encode()  
    
    class InputHandler:

        def __init__(self, terminal):

            self.terminal = terminal

            self.buffer = []
            self.cursor = 0

            self.history_index = 0

        def process_input(self, prompt):

            self.buffer = []
            self.cursor = 0

            self.history_index = len(self.terminal.history)
            
            print(prompt, end='', flush=True)

            while True:

                ch = getch_char()

                #ENTER
                if ch in [b'\r', b'\n']:
                    print()
                    return ''.join(self.buffer)
                
                #BACKSPACE
                elif ch in [b'\x08', b'\x7f']:

                    if self.cursor > 0:

                        self.cursor -= 1
                        self.buffer.pop(self.cursor)
                
                #SETAS (Windows)
                elif ch == b'\xe0':

                    arrow = getch_char()

                    if arrow == b'K':  # Left arrow
                        if self.cursor > 0:
                            self.cursor -= 1
                    elif arrow == b'M':  # Right arrow
                        if self.cursor < len(self.buffer):
                            self.cursor += 1
                    elif arrow == b'H':  # Up arrow
                        self.history_up()
                    elif arrow == b'P':  # Down arrow   
                        self.history_down()

                #SETAS (Linux/Mac)
                elif ch == b'\x1b':

                    next1 = getch_char()
                    next2 = getch_char()

                    if next2 == b'D':  # Left arrow
                        if self.cursor > 0:
                            self.cursor -= 1
                    elif next2 == b'C':  # Right arrow
                        if self.cursor < len(self.buffer):
                            self.cursor += 1
                    elif next2 == b'A':  # Up arrow
                        self._history_up()
                    elif next2 == b'B':  # Down arrow
                        self._history_down()
                
                #TAB
                elif ch == b"\t":
                    self.auto_complete()

                #CARACTERE NORMAL
                else:
                    
                    try:
                        char = ch.decode()
                        self.buffer.insert(self.cursor, char)
                        self.cursor += 1
                    except:
                        pass

                self._render(prompt)

        def _render(self, prompt):
            
            #LIMPA A LINHA
            print("\r", end='')
            print(" " * (len(prompt) + len(self.buffer) + 5), end='')

            #REDESENHA
            print("\r", end='')
            print(prompt + ''.join(self.buffer), end='')

            #POSICIONA O CURSOR

            pos = len(prompt) + self.cursor

            print(f"\r{prompt}{''.join(self.buffer[:self.cursor])}", end='', flush=True)
        
        def _history_up(self):

            if not self.terminal.history:
                return
            
            self.history_index = max(0, self.history_index - 1)
            self.buffer = list(self.terminal.history[self.history_index])
            self.cursor = len(self.buffer)

        def _history_down(self):
            
            if not self.terminal.history:
                return
            
            self.history_index = min(len(self.terminal.history) - 1, self.history_index + 1)
            self.buffer = list(self.terminal.history[self.history_index])
            self.cursor = len(self.buffer)

        def auto_complete(self):

            text = ''.join(self.buffer)

            matches = [cmd for cmd in self.terminal.commands.keys() if cmd.startswith(text)]

            if len(matches) == 1:

                self.buffer = list(matches[0])
                self.cursor = len(self.buffer)
            
            elif len(matches) == 1:

                self.buffer = list(matches[0])
                self.cursor = len(self.buffer)

            elif len(matches) > 1:

                print("\n")
                for m in matches:
                    print(m)

                input("\n Press ENTER to continue...")

            self._render("")