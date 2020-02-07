from flask import Flask
from flask import jsonify
from flask import request

app = Flask(__name__)

registers = [{'id': '111', 'tipo': '1'},
            {'id': '112', 'tipo': '2'},
            {'id': '113', 'tipo': '3'},
            {'id': '114', 'tipo': '1'}]

acessos = []

def html_template(titulo, msg):
    html="""<html>
        <body>
        <h1>{}</h1>
        <p>{}</p>
        </body>
        </html>""".format(titulo, msg)
    return html

@app.route('/', methods=['GET'])
def hello():
    return html_template("Bem vindo a catraca.", " A lista de todos os acessos é dada em /TodosAcessos")

@app.route('/TodosAcessos', methods=['GET'])
def TodosAcessos():
    return jsonify({'Todos acessos': acessos})

@app.route('/getRegisters', methods=['GET'])
def getRegisters():
    return jsonify({'registers' : registers})

@app.route('/novosAcessos', methods=['POST'])
def atualizarEntradas():
    json_request = request.get_json()
    acessos.append(json_request)
    return '0'

if __name__ == "__main__":
    app.run(debug=True, port=5000)
