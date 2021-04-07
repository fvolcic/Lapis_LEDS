class AlphaCounter:

    def __init__(self):
        self.valA = 1
        self.valB = 1
        self.valC = 1
        self.valD = 1

    def get_alpha_val(self):
        return str(chr(self.valA + 64)) + "" + str(chr(self.valB + 65)) + "" + str(chr(self.valC + 65)) + "" + str(chr(self.valD + 65))

    def increment(self):
    
        self.valA += 1

        if self.valA == 27:
            self.valA = 1
            
            self.valB += 1

            if self.valB == 27:
                self.valB = 1

                self.valC += 1

                if self.valC == 27:
                    self.valC = 1

                    self.valD += 1

