"""Main entry point for the CICADA-3301 terminal simulation."""

from kernel.kernel import Kernel

def main():
    """Create the system kernel and start the terminal boot sequence."""
    
    system = Kernel()
    system.boot()

if __name__ == "__main__":
    main()
