
def getRulesFromText(text):
    rules = []
    lines = text.split('\n')
    for line in lines:
        if len(line) == 0:
            continue
        leftside = line[0:line.find('-')]
        rightside = line[line.find('>')+1:len(line)]
        right = rightside.split('|')
        for righ in right:
            rule = []
            rule.append(leftside)
            rule.append(righ)
            rules.append(rule)

    return rules

def stringContainsOneOfList(string, list):
    for element in list:
        if element in string:
            return True

    return False

def bruteForceStart(rules, nonterminals, axiom, iterations):
    generatedStrings = []
    for rule in rules:
        if rule[0] == axiom:
            generatedStrings.append(rule[1])

    result = []
    i=0
    for generatedString in generatedStrings:
        if not stringContainsOneOfList(generatedString, nonterminals):
            result.append(generatedString)
            generatedStrings.pop(i)
        i+=1

    return bruteForce(rules, nonterminals, generatedStrings, iterations-1, result)

def bruteForce(rules, nonterminals, generatedStrings, iterations, result):
    if iterations == 0:
        return result

    newGeneratedStrings = []
    for rule in rules:
        for generatedString in generatedStrings:
            if rule[0] not in generatedString:
                continue
            newGeneratedStrings.append(generatedString.replace(rule[0], rule[1]))

    i=0
    for generatedString in newGeneratedStrings:
        if not stringContainsOneOfList(generatedString, nonterminals):
            result.append(generatedString)
            newGeneratedStrings.pop(i)
        i+=1

    return bruteForce(rules, nonterminals, newGeneratedStrings, iterations-1, result)

file = open("./test.txt", "r")

rules = getRulesFromText(file.read())
axiom = rules[0][0]
print bruteForceStart(rules, ['S'], axiom, 5)
