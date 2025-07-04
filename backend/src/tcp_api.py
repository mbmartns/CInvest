# tcp_api.py
from flask import Flask, jsonify, request # type: ignore
from flask_cors import CORS # type: ignore

app = Flask(__name__)
CORS(app)

last_pattern = {"name": "", "description": "", "status": ""}

@app.route("/api/pattern", methods=["GET"])
def get_pattern():
    return jsonify(last_pattern)

@app.route("/api/pattern", methods=["POST"])
def post_pattern():
    global last_pattern
    data = request.get_json()
    if not data:
        return {"error": "No data"}, 400
    for key in ["name", "description", "status"]:
        if key not in data:
            return {"error": f"Missing {key}"}, 400
    last_pattern = {k: str(data[k]) for k in ["name", "description", "status"]}
    print("Padrão recebido via POST:", last_pattern)
    return {"status": "ok"}


order_opened = {}

@app.route("/api/order_opened", methods=["POST"])
def post_order_opened():
    global order_opened
    data = request.get_json()
    if not data:
        return {"error": "No data"}, 400
    for key in ["type", "volume", "price", "ticket"]:
        if key not in data:
            return {"error": f"Missing {key}"}, 400
    order_opened = {k: str(data[k]) for k in ["type", "volume", "price", "ticket"]}
    print("Ordem aberta recebida via POST:", order_opened)
    return {"status": "ok"}

@app.route("/api/order_opened", methods=["GET"])
def get_order_opened():
    return jsonify(order_opened)

symbol_info = {}

@app.route("/api/symbol", methods=["POST"])
def post_symbol():
    global symbol_info
    data = request.get_json()
    if not data:
        return {"error": "No data"}, 400
    for key in ["symbol", "timeframe"]:
        if key not in data:
            return {"error": f"Missing {key}"}, 400
    symbol_info = {k: str(data[k]) for k in ["symbol", "timeframe"]}
    print("Símbolo recebido via POST:", symbol_info)
    return {"status": "ok"}

@app.route("/api/symbol", methods=["GET"])
def get_symbol():
    return jsonify(symbol_info)


if __name__ == "__main__":
    app.run(port=3001)
