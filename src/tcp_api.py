import threading
from flask import Flask, jsonify, request # type: ignore
from flask_cors import CORS # type: ignore

app = Flask(__name__)
CORS(app)

last_candle = {"open": 0, "high": 0, "low": 0, "close": 0}

@app.route("/api/candle", methods=["GET"])
def get_candle():
    return jsonify(last_candle)

@app.route("/api/candle", methods=["POST"])
def post_candle():
    global last_candle
    data = request.get_json()
    if not data:
        return {"error": "No data"}, 400
    for key in ["open", "high", "low", "close"]:
        if key not in data:
            return {"error": f"Missing {key}"}, 400
    last_candle = {k: float(data[k]) for k in ["open", "high", "low", "close"]}
    print("Candle recebido via POST:", last_candle)
    return {"status": "ok"}

if __name__ == "__main__":
    app.run(port=3001)