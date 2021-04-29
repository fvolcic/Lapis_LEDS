"""Python file for a verbose only printer."""

class VerbosePrinter:

    def __init__(self, verbose=False):
        """The initializer for the verbose printer."""
        self.verbosity = verbose
    
    def set_mode(self, verbose):
        """Change the verbosity of the verbose printer."""
        self.verbosity = verbose
    
    def print(self, message):
        if self.verbosity:
            print(message)