import regex as re
from platform import node

class ASTNode(object):
    stack = {}

    def __init__(self, val=0):
        self.children = []
        self.val = val
        
    def add_child(self, astnode):
        self.children += [astnode]
    
    def get_child(self, index):
        return self.children[index]

    
class ConstNode(ASTNode):
    
    def __init__(self, val):
        super().__init__(val)
    
    def evaluate(self):
        retval = int(self.val)
        return retval


class VarNode(ASTNode):

    def __init__(self, val):
        super().__init__(val)
        ASTNode.stack[val] = self
    
    def evaluate(self):
        retval = self.get_child(0).evaluate()
        return retval

    
class BinOpNode(ASTNode):
            
    opTable = {'LSHIFT': lambda x, y: x << y,
               'RSHIFT': lambda x, y: x >> y,
               'OR': lambda x, y: x | y,
               'AND': lambda x, y: x & y}
    
    def __init__(self, val):
        super().__init__(val)
    
    def evaluate(self):
        retval = BinOpNode.opTable[self.val](self.get_child(0).evaluate(), self.get_child(1).evaluate())
        return retval

        
class UnOpNode(ASTNode):
    
    opTable = {'NOT': lambda x:~x}
    
    def __init__(self, val):
        super().__init__(val)
    
    def evaluate(self):
        retval = UnOpNode.opTable[self.val](self.get_child(0).evaluate())
        return retval

    
def val_to_node(val, node_type):
    if node_type == 0:
        return ConstNode(val)
    elif node_type == 1:
        if (node:=ASTNode.stack.get(val, None)) != None:
            return node
        else:
            return VarNode(val)
    elif node_type == 2:
        return BinOpNode(val)
    elif node_type == 3:
        return UnOpNode(val)
    elif node_type == 4:
        return None

    
def parser(sentence):
    parser_stack = []
    need_rhs = False
    assign = False
    for word in sentence:
        for node_type, val in enumerate(word):
            if val != '':
                node = val_to_node(val, node_type)
                if node_type == 0 or node_type == 1:
                    if need_rhs:
                        parser_stack[0].add_child(node)
                        need_rhs = False
                    elif assign:
                        node.add_child(parser_stack.pop())
                        assign = False
                    else:
                        parser_stack += [node]
                elif node_type == 2:
                    node.add_child(parser_stack.pop())
                    need_rhs = True
                    parser_stack += [node]
                elif node_type == 3:
                    need_rhs = True
                    parser_stack += [node]
                elif node_type == 4:
                    assign = True
    return

def cheap_parser(sentence,val_dict):
    parser_stack = []
    need_rhs = False
    assign = False
    for word in sentence:
        for node_type, val in enumerate(word):
            if val != '':
                node = val_to_node(val, node_type)
                if node_type == 1:
                    if assign:
                        node.add_child(parser_stack.pop())
                        return node
                    if (known_val:=val_dict.get(val,None)) != None:
                        node_type = 0
                        node = val_to_node(known_val, node_type)
                    else:
                        return None
                if node_type == 0:
                    if need_rhs:
                        parser_stack[0].add_child(node)
                        need_rhs = False
                    else:
                        parser_stack += [node]
                elif node_type == 2:
                    node.add_child(parser_stack.pop())
                    need_rhs = True
                    parser_stack += [node]
                elif node_type == 3:
                    need_rhs = True
                    parser_stack += [node]
                elif node_type == 4:
                    assign = True
    
        

if __name__ == "__main__":
    f = open('input_day7_2015.txt')
    strings = f.readlines()
    f.close()
    
    dictionary = re.compile('([0-9]+)|([a-z]+)|(LSHIFT|RSHIFT|OR|AND)|(NOT)|(->)')
    
    my_dict = {}
    changing = True
    while changing:
        changing = False
        for line in strings:
            sentence = dictionary.findall(line)
            node = cheap_parser(sentence,my_dict)
            if node != None and my_dict.get(node.val,None) == None:
                my_dict[node.val] = node.evaluate()
                changing = True
    
    print('a = {}'.format(my_dict['a']))
    
    f = open('input_day7_b_2015.txt')
    strings = f.readlines()
    f.close()
    
    my_dict = {}
    changing = True
    ASTNode.stack = {}
    while changing:
        changing = False
        for line in strings:
            sentence = dictionary.findall(line)
            node = cheap_parser(sentence,my_dict)
            if node != None and my_dict.get(node.val,None) == None:
                my_dict[node.val] = node.evaluate()
                changing = True
    
    print('a = {}'.format(my_dict['a']))
