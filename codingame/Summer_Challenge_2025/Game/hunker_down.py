
def set_hunker_down(ops, agents):
    for op in ops:
        op = op.split(";")
        agent = None
        for i in range(len(op)):
            if i == 0:
                agent = agents[int(op[0])]
                continue
            kind = op[i].split(' ')[0]
            if kind == 'HUNKER_DOWN':
                print(f'{agent["agent_id"]} HUNER_DOWN!')
                agent['HUNKER_DOWN'] = True

def hunker_down(ops1, ops2, agents):
    for i in range(len(agents)):
        if agents[i+1]['wetness'] >= 100: continue
        agents[i+1]['HUNKER_DOWN'] = False
    set_hunker_down(ops1, agents)
    set_hunker_down(ops2, agents)