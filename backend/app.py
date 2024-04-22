import os
import subprocess
import zipfile

from flask import Flask, jsonify, request
from flask_cors import CORS, cross_origin

app = Flask(__name__)
CORS(app)

app.config["MAX_CONTENT_LENGTH"] = 16 * 1024 * 1024 


def run_checker():
    subprocess.run(['../checker/build/main'], cwd='../checker')
    return


@app.route("/upload", methods=["POST"])
def upload_file():
    if "file" not in request.files:
        return jsonify({"error": "No file part"}), 400
    file = request.files["file"]

    
    if file.filename == "":
        return jsonify({"error": "No selected file"}), 400

    if file and file.filename.endswith(".zip"):
        filepath = "solutions.zip"
        file.save(filepath)

        try:
            with zipfile.ZipFile(filepath, "r") as zip_ref:
                extract_path = "../checker/solutions"
                zip_ref.extractall(extract_path)
                os.remove(filepath)
                run_checker()
            return (
                jsonify(
                    {
                        "message": "File uploaded and unzipped successfully",
                        "path": extract_path,
                    }
                ),
                200,
            )
        except zipfile.BadZipFile:
            os.remove(filepath)
            return jsonify({"error": "Failed to unzip file"}), 400
    else:
        return jsonify({"error": "Unsupported file type"}), 400


if __name__ == "__main__":
    app.run(debug=True)
