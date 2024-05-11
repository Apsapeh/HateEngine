from PIL import Image

left   = "skybox/left.jpg"
front  = "skybox/front.jpg"
right  = "skybox/right.jpg"
back   = "skybox/back.jpg"
top    = "skybox/top.jpg"
bottom = "skybox/bottom.jpg"

output = "skybox.jpg"

# Load images
left   = Image.open(left)
front  = Image.open(front)
right  = Image.open(right)
back   = Image.open(back)
top    = Image.open(top)
bottom = Image.open(bottom)

# Create a new image
skybox = Image.new("RGB", (left.width * 4, left.height * 3))

# Paste images
skybox.paste(left,   (0, left.height))
skybox.paste(front,  (left.width, front.height))
skybox.paste(right,  (left.width * 2, right.height))
skybox.paste(back,   (left.width * 3, back.height))
skybox.paste(top,    (left.width, 0))
skybox.paste(bottom, (left.width, bottom.height * 2))

# Mirror the top and bottom
top = top.transpose(Image.FLIP_LEFT_RIGHT)
bottom = bottom.transpose(Image.FLIP_LEFT_RIGHT)
skybox.paste(top,    (0, 0))
skybox.paste(top,    (top.width * 2, 0))
skybox.paste(bottom, (0, bottom.height * 2))
skybox.paste(bottom, (bottom.width * 2, bottom.height * 2))

# Save the image
skybox.save(output)