"""Build static HTML site from directory of HTML templates and plain files."""
import json
import sys
import shutil
from pathlib import Path
import click
import jinja2
from verbose_printer import VerbosePrinter

printer = VerbosePrinter(False)

@click.command()
@click.argument('input_dir')
@click.option('--output', '-o', default='no file location specified',
              help='Output directory.')
@click.option('--verbose', '-v', is_flag=True, help='printer.print more output.')
def cli_inputs(input_dir, output, verbose):
    """Templated static website generator."""
    if output == 'no file location specified':
        output = input_dir+"/html"
    printer.set_mode(verbose)
    #printer = VerbosePrinter(verbose)
    parse_program(input_dir, output)


def main():
    """Top level command line interface."""
    cli_inputs(None, None, None)


def parse_program(input_dir, output):
    """Logic behinf the program html parser."""
    # variables for the enviroment and the file system

    template_env = jinja2.Environment(
        loader=jinja2.FileSystemLoader("PaserGenerator/templates"),
        #autoescape=jinja2.select_autoescape(['html', 'cpp']),
    )
    
    config = generate_action_keys_from_files(Path(input_dir))

    printer.print(template_env.list_templates())

    template = template_env.get_template('createAction.cpp')
    
    printer.print('Writing Output to: {}'.format("createAction.cpp"))
    (input_dir/Path('createAction.cpp')).unlink(missing_ok=True)
    (input_dir/Path('createAction.cpp')).touch()
    (input_dir/Path('createAction.cpp')).write_text(template.render(config))


def generate_action_keys_from_files(file_dir):
    """
    This will go through each file, determine if the file is
    an LED action file, and if so create an entry for it.

    file_dir must be a Path object
    """
    
    #a config dictionary for the 
    config = { 'actionclasses' : {},
                'filenames' : [] }


    customs_path = Path("PaserGenerator")/Path("custom_parsing")/Path("customs.cpp")
    printer.print("scanning customs")
    with open(customs_path) as f: 
        line = ""
        while "#endfile" not in line:
            line = f.readline()
            if "ACTIONKEY" in line: 
                actionkey = line[line.index(":")+1:line.index(":")+5]
                actionkey = actionkey.replace("\n", "")
                line = f.readline()

                classname = line[line.index(":")+1:len(line)-1]
                classname.rstrip("\n")
                line = f.readline()

                dependencies = [] 
                while "#enddependencies" not in line:
                    #dependencies.append(line)
                    config['filenames'].append(line.replace("\n", ""))
                    line = f.readline()
                
                line = f.readline()
                line = f.readline()
                custom_code = ""
                while "#funcend" not in line: 
                    custom_code += line
                    line = f.readline()
                
                printer.print("Found: " + classname + " in customs. Key:" + actionkey)
                
                if actionkey[0] not in config['actionclasses']:
                    config['actionclasses'][actionkey[0]] =  {}
                
                config['actionclasses'][actionkey[0]][ classname ] = { 'action_key' : actionkey, 
                                                                        'custom_function' : True, 
                                                                        'dependencies' : dependencies,
                                                                        'custom_code' : custom_code }


    #ensure proper path type
    assert type(file_dir) == type(Path())

    
    for file in file_dir.iterdir():
        with open(file) as f:
            first_line = f.readline()
            if "LEDACTIONFILE" in first_line:
                key_line = f.readline()        
                class_line = f.readline().replace(" ", "")
                while not ":publicLED_Action" in class_line:
                    class_line = f.readline().replace(" ", "")
                classname = class_line[0:class_line.index(":")].strip("class")
                actionkey = key_line[key_line.index(":")+1:key_line.index(":")+5]
                printer.print( "Found: " + classname + "Key: " + actionkey)
                if actionkey[0] not in config['actionclasses']:
                    config['actionclasses'][actionkey[0]] =  {}


                #printer.print("attempting to add: " + classname + " to letter: " + actionkey[0])
                #printer.print(config['actionclasses'][actionkey[0]])  
                config['actionclasses'][actionkey[0]][ classname ] = { 'action_key' : actionkey, 
                                                                        'custom_function' : False, 
                                                                        'dependencies' : [],
                                                                        'custom_code' : None }

                #printer.print (config['actionclasses'][actionkey[0]])                                                        
                config['filenames'].append( file.name )

                #printer.print(config)
    
    #printer.print(config) 
    return config



if __name__ == "__main__":
    main()

