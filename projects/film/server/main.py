from fastapi import FastAPI, File, UploadFile, HTTPException
from fastapi.responses import JSONResponse
import os
import uuid
from pathlib import Path
from dotenv import load_dotenv
from google import genai
from google.genai import types
import glob
import base64

# Load environment variables
load_dotenv()

app = FastAPI(title="Image and Sound Upload API")

# Initialize Gemini client
client = genai.Client(api_key=os.getenv("GEMINI_API_KEY"))

# Create images directory if it doesn't exist
os.makedirs("images", exist_ok=True)

@app.post("/upload_image")
async def upload_image(image: UploadFile = File(...)):
    """
    Upload an image and generate a detailed description using Gemini AI
    """
    try:
        # Validate file type
        if not image.content_type.startswith("image/"):
            raise HTTPException(status_code=400, detail="File must be an image")
        
        # Extract filename without extension to use as folder name
        if image.filename:
            # Remove .jpg extension if present
            folder_name = image.filename.replace('.jpg', '').replace('.jpeg', '').replace('.png', '')
        else:
            # Fallback to UUID if no filename
            folder_name = str(uuid.uuid4())
        
        folder_path = Path(f"images/{folder_name}")
        folder_path.mkdir(parents=True, exist_ok=True)
        
        # Save the image
        image_path = folder_path / "input.jpg"
        with open(image_path, "wb") as buffer:
            content = await image.read()
            buffer.write(content)
        
        # Analyze image with Gemini
        try:
            # Read the image file as bytes
            with open(image_path, "rb") as img_file:
                image_data = img_file.read()
            
            # Create image part for Gemini
            image_part = types.Part.from_bytes(
                data=image_data,
                mime_type="image/jpeg"
            )
            
            # Generate description
            response = client.models.generate_content(
                model="gemini-2.0-flash-001",
                contents=[
                    "Analyze this image in detail. Describe what you see, including objects, people, settings, colors, emotions, and any notable details. Be as descriptive as possible in under 200 words.",
                    image_part
                ]
            )
            
            description = response.text
            
            # Save description to file
            description_path = folder_path / "description.txt"
            with open(description_path, "w", encoding="utf-8") as desc_file:
                desc_file.write(description)
            
            return JSONResponse(
                content={
                    "message": "Image uploaded and analyzed successfully",
                    "folder_name": folder_name,
                    "description": description
                }
            )
            
        except Exception as e:
            # Clean up if AI analysis fails
            if image_path.exists():
                image_path.unlink()
            if folder_path.exists():
                folder_path.rmdir()
            print(f"AI analysis failed: {str(e)}")
            raise HTTPException(status_code=500, detail=f"AI analysis failed: {str(e)}")
    
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Upload failed: {str(e)}")

@app.post("/upload_audio")
async def upload_sound(file: UploadFile = File(...)):
    """
    Upload a sound file and find the most suitable image based on the audio content
    """
    try:
        # Validate file type (basic audio types)
        if not file.content_type.startswith("audio/"):
            raise HTTPException(status_code=400, detail="File must be an audio file")
        
        # Create sounds directory if it doesn't exist
        os.makedirs("sounds", exist_ok=True)
        
        # Generate unique filename for the audio file
        audio_id = str(uuid.uuid4())
        audio_filename = f"{audio_id}.wav"
        audio_path = Path(f"sounds/{audio_filename}")
        
        # Read and save audio file
        audio_content = await file.read()
        with open(audio_path, "wb") as audio_file:
            audio_file.write(audio_content)
        
        print(f"Audio saved to: {audio_path}")
        
        # Read all image descriptions
        image_descriptions = {}
        images_dir = Path("images")
        
        if not images_dir.exists():
            raise HTTPException(status_code=404, detail="No images found")
        
        # Collect all image descriptions
        for folder in images_dir.iterdir():
            if folder.is_dir():
                desc_file = folder / "description.txt"
                if desc_file.exists():
                    with open(desc_file, "r", encoding="utf-8") as f:
                        image_descriptions[folder.name] = f.read()
        
        if not image_descriptions:
            raise HTTPException(status_code=404, detail="No image descriptions found")
        
        # Create audio part for Gemini
        audio_part = types.Part.from_bytes(
            data=audio_content,
            mime_type=file.content_type
        )
        
        # Create prompt with all image descriptions
        descriptions_text = "\n\n".join([
            f"Image {img_id}: {desc}" 
            for img_id, desc in image_descriptions.items()
        ])
        
        prompt = f"""
        Listen to this audio and analyze what the person is saying or what sounds are present.
        
        Then, from the following image descriptions, choose the most suitable image that matches the audio content:
        
        {descriptions_text}
        
        Respond with only the image ID (folder name) that best matches the audio content.
        """
        
        # Get response from Gemini
        response = client.models.generate_content(
            model="gemini-2.0-flash-001",
            contents=[
                prompt,
                audio_part
            ]
        )
        
        # Extract image ID from response and clean it
        selected_image = response.text.strip()
        
        # Remove quotes if present
        selected_image = selected_image.strip('"\'')
        
        # Validate the selected image exists
        if selected_image not in image_descriptions:
            # If exact match fails, try to find partial match
            for img_id in image_descriptions.keys():
                if img_id in selected_image:
                    selected_image = img_id
                    break
            else:
                # If no match found, return the first available image
                selected_image = list(image_descriptions.keys())[0]
                
        print(f"Selected image based on audio: {selected_image}")
        # Return just the image name as raw text
        return selected_image
        
    except Exception as e:
        print(f"Audio processing failed: {str(e)}")
        raise HTTPException(status_code=500, detail=f"Audio processing failed: {str(e)}")

@app.get("/")
async def root():
    return {"message": "Image and Sound Upload API is running"}

@app.get("/images")
async def list_images():
    """List all uploaded images with their descriptions"""
    try:
        images_dir = Path("images")
        if not images_dir.exists():
            return {"images": []}
        
        images = []
        for folder in images_dir.iterdir():
            if folder.is_dir():
                desc_file = folder / "description.txt"
                if desc_file.exists():
                    with open(desc_file, "r", encoding="utf-8") as f:
                        description = f.read()
                    images.append({
                        "id": folder.name,
                        "description": description
                    })
        
        return {"images": images}
    
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Failed to list images: {str(e)}")

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)