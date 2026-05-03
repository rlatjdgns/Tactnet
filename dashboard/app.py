from flask import Flask, jsonify, render_template
import json
import os

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/data')
def data():
    try:
        with open('/tmp/tactnet_data.json', 'r') as f:
            return jsonify(json.load(f))
    except:
        return jsonify({"error": "no data"})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)