﻿using System;
using Flood.GUI.Controls;

namespace Flood.GUI.ControlInternal
{
    /// <summary>
    /// Splitter bar.
    /// </summary>
    public class SplitterBar : Dragger
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="SplitterBar"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public SplitterBar(Control parent)
            : base(parent)
        {
            Target = this;
            RestrictToParent = true;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skins.Skin skin)
        {
            if (ShouldDrawBackground)
                skin.DrawButton(this, true, false, IsDisabled);
        }

        /// <summary>
        /// Lays out the control's interior according to alignment, padding, dock etc.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Layout(Skins.Skin skin)
        {
            MoveTo(X, Y);
        }
    }
}
