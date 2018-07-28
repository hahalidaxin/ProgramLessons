

class Nation:
    def __init__(self, name, continent, population, area):
        self.name = name
        self.continent = continent
        self.population = population
        self.area = area

    def popDensity(self):
        return float(self.population)/float(self.area)
