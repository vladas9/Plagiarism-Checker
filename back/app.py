import os
import zipfile

from flask import Flask, jsonify, request

app = Flask(__name__)

# Configure the maximum upload size (optional)
app.config["MAX_CONTENT_LENGTH"] = 16 * 1024 * 1024  # 16 MB limit


@app.route("/upload", methods=["POST"])
def upload_file():
    # Check if the post request has the file part
    if "file" not in request.files:
        return jsonify({"error": "No file part"}), 400
    file = request.files["file"]

    # If the user does not select a file, the browser submits an
    # empty file without a filename.
    if file.filename == "":
        return jsonify({"error": "No selected file"}), 400

    if file and file.filename.endswith(".zip"):
        # Save the file
        save_path = "./"
        filepath = os.path.join(save_path, file.filename)
        file.save(filepath)

        # Unzip the file
        try:
            with zipfile.ZipFile(filepath, "r") as zip_ref:
                # Extract all the contents into directory
                extract_path = os.path.join(save_path, "unzipped")
                zip_ref.extractall(extract_path)
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
            return jsonify({"error": "Failed to unzip file"}), 400
    else:
        return jsonify({"error": "Unsupported file type"}), 400


if __name__ == "__main__":
    app.run(debug=True)