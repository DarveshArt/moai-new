----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
text = 'The quick <c:f70>brown<c> fox jumps over the <c:7f3>lazy<c> dog.'

textbox = MOAITextBox.new ()
font = MOAIFont.new ()

font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 12, 163 )
textbox:setString ( text )
textbox:setFont ( font )
textbox:setTextSize ( 12, 163 )
textbox:setRect ( -150, -230, 150, 230 )
textbox:setYFlip ( true )
layer:insertProp ( textbox )

